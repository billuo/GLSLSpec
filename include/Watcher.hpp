#pragma once

#include "Expected.hpp"
#include "Thread.hpp"
#include "FileSystem.hpp"

#include <unordered_map>


enum class FileType : int {
    Shader,
    Image,
    Geometry,
    Dependency,
};

struct DynamicFile {
    DynamicFile(const std::string& path, FileType type) noexcept;

    DynamicFile(const DynamicFile& obj) : m_path(obj.m_path), m_type(obj.m_type), tag(obj.tag)
    {}

    const auto& path() const
    { return m_path; }

    auto type() const
    { return m_type; }

    /// @brief Check for update and mark as modified if needed.
    bool check_update() const;

    bool operator<(const DynamicFile& rhs) const
    { return m_path < rhs.m_path; }

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

/// Thread watching a set of files
class Watcher {
  public:
    explicit Watcher(const std::vector<DynamicFile>& to_watch);

    ~Watcher();

    void watch(const FS::path& path, FileType type);

    void unwatch(const FS::path& path, FileType type);

    using Callback = std::function<void(const DynamicFile&)>;

    Callback set_callback(Callback callback)
    {
        std::swap(callback, m_callback);
        return callback;
    }

    expected<DynamicFile, std::string> find(const FS::path& path);

  private:
    bool m_watching = true;
    Callback m_callback = [](const DynamicFile& path)
    {};

    std::unordered_map<FS::path, DynamicFile> m_watching_files;
    std::mutex mutex_watching_files;
};
