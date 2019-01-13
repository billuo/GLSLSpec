/**
 * @File Watcher.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Watcher.hpp>
#include <Utility/Log.hpp>
#include <Utility/Enumeration.hpp>
#include <fstream>


DEFINE_ENUMERATION_DATABASE(FileType) {
        {FileType::Shader,     "Shader"},
        {FileType::Image,      "Image"},
        {FileType::Geometry,   "Geometry"},
        {FileType::Dependency, "Dependency"},
        {FileType::None,       "NONE"},
};

Watcher::Watcher(const std::vector<ImportedFile>& to_watch) :
        m_thread([this]()
                 {
                     std::vector<ImportedFile> to_erase;
                     while (m_watching) {
                         {
                             std::lock_guard guard1(mutex_watching_files);
                             for (auto&&[_, file] : m_watching_files) {
                                 if (file.check_update()) {
                                     std::lock_guard guard2(mutex_updated);
                                     m_updated.insert(file);
                                 } else {
                                     to_erase.push_back(file);
                                 }
                             }
                             for (auto&& file : to_erase) {
                                 m_watching_files.erase({file.path(), file.tag});
                             }
                         }
                         sleep_for_ms(250);
                     }
                 })
{
    std::lock_guard guard(mutex_watching_files);
    for (auto file : to_watch) {
        if (file.path().empty()) {
            continue; // XXX non-existent path is emptied during preprocessing
        }
        m_watching_files.emplace(std::make_pair(file.path(), file.tag), std::move(file));
    }
}

Watcher::~Watcher()
{
    m_watching = false;
    m_thread.join();
}

void
Watcher::watch(const FS::path& path, FileType type, const std::string& tag)
{
    if (path.empty()) {
        Log::e("Can not watch empty path.");
        return;
    }
    std::lock_guard guard(mutex_watching_files);
    auto&&[it, result] = m_watching_files.emplace(std::make_pair(path, tag), ImportedFile(path, type, tag));
    if (!result) {
        Log::w("Already watching {}", path);
    }
}

void
Watcher::unwatch(const FS::path& path, const std::string& tag)
{
    if (path.empty()) {
        Log::e("Can not unwatch empty path.");
        return;
    }
    std::lock_guard guard(mutex_watching_files);
    m_watching_files.erase(std::make_pair(path, tag));
}

expected<ImportedFile, std::string>
Watcher::find(const FS::path& path, const std::string& tag)
{
    std::lock_guard guard(mutex_watching_files);
    auto it = m_watching_files.find(std::make_pair(path, tag));
    if (it == m_watching_files.end()) {
        return make_unexpected(std::string("Not yet watching"));
    } else {
        return it->second;
    }
}

std::set<std::pair<FS::path, std::string>>
Watcher::files() const
{
    std::lock_guard guard(mutex_watching_files);
    std::set<std::pair<FS::path, std::string>> ret;
    for (auto&&[pair, file] : m_watching_files) {
        ret.insert(pair);
    }
    return ret;
}

ImportedFile::ImportedFile(const std::string& path, FileType type, std::string tag) noexcept
        : tag(std::move(tag)), m_type(type)
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
ImportedFile::fetch() const
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
ImportedFile::check_update() const
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

