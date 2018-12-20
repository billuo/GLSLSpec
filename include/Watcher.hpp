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
#include <set>


enum class FileType : int {
    Shader,
    Image,
    Geometry,
    Dependency,
    None,
};

struct ImportedFile {
    ImportedFile() = default;
    ImportedFile(const std::string& path, FileType type, std::string tag) noexcept;

    ImportedFile(const ImportedFile& obj) : tag(obj.tag), m_path(obj.m_path), m_type(obj.m_type)
    {}

    const auto& path() const
    { return m_path; }

    auto type() const
    { return m_type; }

    /// @brief Check for update and mark as modified if it is.
    bool check_update() const;

    bool operator<(const ImportedFile& rhs) const
    {
        if (tag < rhs.tag)
            return true;
        if (rhs.tag < tag)
            return false;
        return m_path < rhs.m_path;
    }

    bool operator==(const ImportedFile& rhs) const
    { return tag == rhs.tag && m_path == rhs.m_path; }

    bool operator!=(const ImportedFile& rhs) const
    { return !(rhs == *this); }

    /// @brief Fetch the file content of this file.
    /// @return Content of the file as a string
    expected<std::string, std::string> fetch() const;

    /// User defined tag
    std::string tag;

  private:
    /// Path to the file
    FS::path m_path{};
    /// Type of file.
    FileType m_type{FileType::None};
    /// Time point when the file was last modified.
    mutable FS::ModifiedTimePoint m_last_modified{};
};

namespace std {
template <>
struct hash<std::pair<FS::path, std::string>> {
    std::size_t operator()(const std::pair<FS::path, std::string>& value) const noexcept
    { return std::hash<std::string>{}(value.first.string()) ^ std::hash<std::string>{}(value.second); }
};

template <>
struct hash<ImportedFile> {
    std::size_t operator()(const ImportedFile& path) const noexcept
    { return std::hash<std::string>{}(path.path().string()); }
};
}

/// Thread watching a set of files
class Watcher {
  public:
    explicit Watcher(const std::vector<ImportedFile>& to_watch);

    ~Watcher();

    void watch(const FS::path& path, FileType type, const std::string& tag);

    void unwatch(const FS::path& path, FileType type, const std::string& tag);

    expected<ImportedFile, std::string> find(const FS::path& path, const std::string& tag);

    /// Move out the set of currently updated files.
    /// @warning Don't ignore its return value unless you really mean to.
    std::unordered_set<ImportedFile> updated()
    {
        std::lock_guard guard(mutex_updated);
        return std::move(m_updated);
    }

    std::set<std::pair<FS::path, std::string>> files() const;

  private:
    std::unordered_map<std::pair<FS::path, std::string>, ImportedFile> m_watching_files;
    mutable std::mutex mutex_watching_files;
    std::unordered_set<ImportedFile> m_updated;
    std::mutex mutex_updated;

    std::atomic_bool m_watching = true;
    std::thread m_thread; // XXX MUST be initialized after mutexes and atomics it might uses.
};


