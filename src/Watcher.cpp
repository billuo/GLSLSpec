#include "Watcher.hpp"
#include "Utility/Enumeration.hpp"
#include "Sandbox.hpp"
#include "Utility/Debug.hpp"

#include <fstream>
#include <iterator>
#include <Watcher.hpp>


DEFINE_ENUMERATION_DATABASE(FileType) {
        {FileType::Shader,     "Fragment shader"},
        {FileType::Image,      "Image"},
        {FileType::Geometry,   "Geometry"},
        {FileType::Dependency, "Dependency"},
};

Watcher::Watcher(const std::vector<DynamicFile>& to_watch)
{
    std::thread([&]()
                {
                    while (this->m_watching) {
                        std::lock_guard guard(mutex_watching_files);
                        for (auto&&[path, file] : m_watching_files) {
                            if (file.check_update()) {
                                if (sandbox) {
                                    sandbox->on_update(file);
                                }
                                // m_callback(file);
                            }
                        }
                    }
                    sleep_for_ms(500);
                }).detach();
    std::lock_guard guard(mutex_watching_files);
    for (auto file : to_watch) {
        DEBUG("{} added to watch", file.path());
        m_watching_files.emplace(file.path(), std::move(file));
    }
}

Watcher::~Watcher()
{ m_watching = false; }

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

DynamicFile::DynamicFile(const std::string& path, FileType type) noexcept
        : m_path(std::move(FS::canonical(path))), m_type(type), m_last_modified(FS::last_write_time(m_path))
{}

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
    auto last_modified = FS::last_write_time(m_path);
    if (last_modified > m_last_modified) {
        DEBUG("Found {} modified", m_path);
        m_last_modified = last_modified;
        return true;
    }
    return false;
}

