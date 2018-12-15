/**
 * @File Watcher.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Utility/Expected.hpp>
#include <Utility/Thread.hpp>
#include <FileSystem.hpp>
#include <unordered_map>
#include <unordered_set>


enum class FileType : int {
    Shader,
    Image,
    Geometry,
    Dependency,
};

struct DynamicFile {
    DynamicFile(const std::string& path, FileType type) noexcept;

    DynamicFile(const DynamicFile& obj) : tag(obj.tag), m_path(obj.m_path), m_type(obj.m_type)
    {}

    const auto& path() const
    { return m_path; }

    auto type() const
    { return m_type; }

    /// @brief Check for update and mark as modified if needed.
    bool check_update() const;

    bool operator<(const DynamicFile& rhs) const
    { return m_path < rhs.m_path; }

    bool operator==(const DynamicFile& rhs) const
    { return m_path == rhs.m_path; }

    bool operator!=(const DynamicFile& rhs) const
    { return !(rhs == *this); }

    /// @brief Fetch the file content of this file.
    /// @param latest Set to true to ensure reading the latest file content.
    /// @return Content of the file as a string
    expected<std::string, std::string> fetch() const;

    /// User defined tag
    std::string tag;

  private:
    /// Path to the file
    FS::path m_path;
    /// Type of file.
    FileType m_type;
    /// Time point when the file was last modified.
    mutable FS::ModifiedTimePoint m_last_modified;
};

namespace std {
template <>
struct hash<DynamicFile> {
    std::size_t operator()(const DynamicFile& path) const noexcept
    { return std::hash<std::string>{}(path.path().string()); }
};
}

/// Thread watching a set of files
class Watcher {
  public:
    explicit Watcher(const std::vector<DynamicFile>& to_watch);

    ~Watcher();

    void watch(const FS::path& path, FileType type);

    void unwatch(const FS::path& path, FileType type);

    expected<DynamicFile, std::string> find(const FS::path& path);

    /// Move out the set of currently updated files.
    /// @warning Don't ignore its return value unless you really mean to.
    std::unordered_set<DynamicFile> updated()
    {
        std::lock_guard guard(mutex_updated);
        return std::move(m_updated);
    }

  private:
    std::unordered_map<FS::path, DynamicFile> m_watching_files;
    std::mutex mutex_watching_files;
    std::unordered_set<DynamicFile> m_updated;
    std::mutex mutex_updated;

    std::atomic_bool m_watching = true;
    std::thread m_thread; // XXX MUST be initialized after mutexes and atomics it might uses.
};


