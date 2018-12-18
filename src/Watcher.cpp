/**
 * @File Watcher.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Watcher.hpp>
#include <Sandbox.hpp>
#include <fstream>


DEFINE_ENUMERATION_DATABASE(FileType) {
        {FileType::Shader,     "Fragment shader"},
        {FileType::Image,      "Image"},
        {FileType::Geometry,   "Geometry"},
        {FileType::Dependency, "Dependency"},
};

Watcher::Watcher(const std::vector<DynamicFile>& to_watch) :
        m_thread([this]()
                 {
                     while (m_watching) {
                         {
                             std::lock_guard guard1(mutex_watching_files);
                             for (auto&&[_, file] : m_watching_files) {
                                 if (file.check_update()) {
                                     std::lock_guard guard2(mutex_updated);
                                     m_updated.insert(file);
                                 }
                             }
                         }
                         sleep_for_ms(250);
                     }
                 })
{
    std::lock_guard guard(mutex_watching_files);
    for (auto file : to_watch) {
        if (file.path().empty()) {
            continue; // XXX non-existent path is emptied during pre-processing
        }
        m_watching_files.emplace(file.path(), std::move(file));
    }
}

Watcher::~Watcher()
{
    m_watching = false;
    m_thread.join();
}

void
Watcher::watch(const FS::path& path, FileType type)
{
    std::lock_guard guard(mutex_watching_files);
    auto&&[it, result] = m_watching_files.emplace(path, DynamicFile(path, type));
    if (!result) {
        Log::w("Already watching {}", path);
    } else {
        DEBUG("{} added to watch", path);
    }
}

void
Watcher::unwatch(const FS::path& path, FileType type)
{
    std::lock_guard guard(mutex_watching_files);
    auto n = m_watching_files.erase(path);
    if (n == 0) {
        Log::w("File to stop watching({}) was unwatched.", path);
    } else {
        DEBUG("{} no longer watching", path);
    }
}

expected<DynamicFile, std::string>
Watcher::find(const FS::path& path)
{
    std::lock_guard guard(mutex_watching_files);
    auto it = m_watching_files.find(path);
    if (it == m_watching_files.end()) {
        return make_unexpected(std::string("Not yet watching"));
    } else {
        return it->second;
    }
}

DynamicFile::DynamicFile(const std::string& path, FileType type) noexcept : m_type(type)
{
    if (auto&& ex = FS::canonical(path)) {
        m_path = *ex;
    } else {
        Log::e("{}", ex.error());
    }
    if (auto&& ex = FS::last_write_time(m_path)) {
        m_last_modified = *ex;
    } else {
        Log::e("{}", ex.error());
    }
}

expected<std::string, std::string>
DynamicFile::fetch() const
{
    std::ifstream ifs;
    std::string content;
    ifs.exceptions(std::ifstream::badbit);
    try {
        ifs.open(m_path);
        content.assign(std::istreambuf_iterator{ifs}, {});
    } catch (std::ifstream::failure& e) {
        return make_unexpected(std::string(e.what()));
    }
    return content;
}

bool
DynamicFile::check_update() const
{
    if (auto&& ex = FS::last_write_time(m_path)) {
        if (*ex > m_last_modified) {
            m_last_modified = *ex;
            return true;
        }
    } else {
        Log::e("{}", ex.error());
    }
    return false;
}

