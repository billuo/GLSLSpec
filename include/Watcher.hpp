#pragma once

#include "Thread.hpp"
#include "FileSystem.hpp"

#include <unordered_map>


enum class FileType : int {
    Shader,
    Image,
    Geometry,
    Dependency,
};

/// Thread watching a set of files
class Watcher {
  public:
    struct DynamicFile {
        DynamicFile(const std::string& path, FileType type)
                : m_path(std::move(FS::canonical(path))), m_type(type), m_last_modified(FS::last_write_time(path))
        {}

        DynamicFile(const DynamicFile& obj) : m_path(obj.m_path), m_type(obj.m_type)
        {}

        const auto& path() const
        { return m_path; }

        auto type() const
        { return m_type; }

        /// Check for update.
        void check_update() const;

        /// Read content of file into string.
        std::string fetch(bool latest = true) const;

        /// User defined tag
        std::string tag;

      private:
        /// Path to the file
        FS::path m_path;
        /// Type of file.
        FileType m_type;
        /// Flag signaling if modified since last read (and cache)
        mutable std::atomic_bool m_modified = true;
        /// Time point when the file was last modified.
        mutable FS::ModifiedTimePoint m_last_modified;
        /// Cached file content.
        mutable std::string m_cached;
    };

    explicit Watcher(const std::vector<DynamicFile>& to_watch);

    ~Watcher();

    void watch(const FS::path& path, FileType type);

    void unwatch(const FS::path& path, FileType type);

  private:
    bool m_watching = true;

    std::unordered_map<FS::path, DynamicFile> m_watching_files;
    std::mutex mutex_WatchingFiles;
};

