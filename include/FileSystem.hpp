/**
 * @File FileSystem.hpp
 * @author Zhen Luo 461652354@qq.com
 */

#pragma once

#include "Utility/Expected.hpp"
#include <vector>
#include <chrono>
#include <experimental/filesystem>
// TODO some newer compiler supports directly including <filesystem>. detect them.


namespace FS {

namespace details {
using namespace std::experimental::filesystem;
}

using path = details::path;

using ModifiedTimePoint = decltype(details::last_write_time(details::path()));

inline auto
extension(const FS::path& file) noexcept
{ return file.extension().string(); }

/// noexcept wrapper for fs::last_write_time();
inline expected<ModifiedTimePoint, std::string>
last_write_time(const FS::path& path) noexcept
{
    try {
        return details::last_write_time(path);
    } catch (std::exception& e) {
        return make_unexpected(e.what());
    }
}

/// noexcept wrapper for fs::exists()
inline expected<bool, std::string>
exists(const std::string& name) noexcept
{
    try {
        return details::exists(name);
    } catch (std::exception& e) {
        return make_unexpected(e.what());
    }
}

/// noexcept wrapper for fs::canonical()
inline expected<FS::path, std::string>
canonical(const std::string& path) noexcept
{
    try {
        return details::canonical(path);
    } catch (std::exception& e) {
        return make_unexpected(e.what());
    }
}

// using FileList =  std::vector<FS::path>;
//
// /// Load the content of a shader into a string
// /// @param file_name The <B>canonical</B> path to the shader.
// /// @param include_folders A list of folders available to include files from
// /// @param [out] into The string to hold the file content
// /// @param [out] dependencies A list of <B>canonical</B> paths to actual file dependencies, including @p file_name.
// /// @returns True if loaded successfully. Otherwise false.
// /// @details It recurse but can detect and ignore on cyclic include in shader code. But don't do that anyway!
// /// @note The include mechanism here is <B>NOT</B> that in ARB_shading_language_include.
// /// Concretely, here we really do query the filesystem on OS for the files we include,
// /// while the extension can only "#include" files already uploaded to OpenGL.
// /// @sa <a href="https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt">ARB_shading_language_include Registry</a>
// bool
// load_shader(const FS::path& file_name, std::string& into, FileList& dependencies);

} // namespace FS

namespace std {
template <>
struct hash<FS::path> {
    std::size_t operator()(const FS::path& path) const noexcept
    { return std::hash<std::string>{}(path.string()); }
};
}
