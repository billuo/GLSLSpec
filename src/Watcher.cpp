#include "Watcher.hpp"
#include "Enumeration.hpp"
#include "Log.hpp"

#include <fstream>
#include <iterator>


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
                        std::lock_guard guard(mutex_WatchingFiles);
                        for (auto&&[path, file] : m_watching_files) {
                            file.check_update();
                        }
                    }
                    sleep_for_ms(500.0f);
                }).detach();
    std::lock_guard guard(mutex_WatchingFiles);
    for (auto& file : to_watch) {
        Log::d("{} added to watch", file.path());
        m_watching_files.emplace(file.path(), DynamicFile(file.path(), file.type()));
    }
}

Watcher::~Watcher()
{ m_watching = false; }

void
Watcher::watch(const FS::path& path, FileType type)
{
    std::lock_guard guard(mutex_WatchingFiles);
    auto&&[it, result] = m_watching_files.emplace(path, DynamicFile(path, type));
    if (!result) {
        Log::w("Already watching {}", path);
    } else {
        Log::d("{} added to watch", path);
    }
}

void
Watcher::unwatch(const FS::path& path, FileType type)
{
    std::lock_guard guard(mutex_WatchingFiles);
    auto n = m_watching_files.erase(path);
    if (n == 0) {
        Log::w("File to stop watching({}) was unwatched.", path);
    } else {
        Log::d("{} no longer watching", path);
    }
}

std::string
Watcher::DynamicFile::fetch(bool latest) const
{
    if (latest) {
        check_update();
    }
    if (!m_modified) {
        return m_cached;
    }
    m_modified = false;
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        ifs.open(m_path);
        m_cached = std::move(std::string(std::istreambuf_iterator{ifs}, {}));
    } catch (std::ifstream::failure& e) {
        Log::e("Failed to open file {}: {}", m_path, e.what());
    }
    return m_cached;
}

void
Watcher::DynamicFile::check_update() const
{
    auto last_modified = FS::last_write_time(m_path);
    if (last_modified > m_last_modified) {
        m_last_modified = last_modified;
        m_modified = true;
    }
}

