/**
 * @File FileSystem.hpp
 * @author Zhen Luo 461652354@qq.com
 */

// TODO make those wrappers return expected<...>

#pragma once

#include <vector>
#include <chrono>
#include <experimental/filesystem>
// TODO some newer compiler supports directly including <filesystem>. detect them.


namespace FS {
using path = std::experimental::filesystem::path;
namespace details = std::experimental::filesystem;


using ModifiedTimePoint = decltype(details::last_write_time(details::path()));

/// noexcept wrapper for fs::last_write_time();
ModifiedTimePoint
last_write_time(const FS::path& path) noexcept;

inline auto
extension(const FS::path& file)
{ return file.extension().string(); }

/// noexcept wrapper for fs::exists()
bool
exists(const std::string& name) noexcept;

/// noexcept wrapper for fs::canonical()
FS::path
canonical(const std::string& path) noexcept;

struct WatchFile;

using FileList =  std::vector<FS::path>;
using WatchFileList= std::vector<WatchFile>;

/// Load the content of a shader into a string
/// @param file_name The <B>canonical</B> path to the shader.
/// @param include_folders A list of folders available to include files from
/// @param [out] into The string to hold the file content
/// @param [out] dependencies A list of <B>canonical</B> paths to actual file dependencies, including @p file_name.
/// @returns True if loaded successfully. Otherwise false.
/// @details It recurse but can detect and ignore on cyclic include in shader code. But don't do that anyway!
/// @note The include mechanism here is <B>NOT</B> that in ARB_shading_language_include.
/// Concretely, here we really do query the filesystem on OS for the files we include,
/// while the extension can only "#include" files already uploaded to OpenGL.
/// @sa <a href="https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_include.txt">ARB_shading_language_include Registry</a>
bool
load_shader(const FS::path& file_name, std::string& into, FileList& dependencies);

} // namespace FS

namespace std {
template <>
struct hash<FS::path> {
    std::size_t operator()(const FS::path& path) const noexcept
    { return std::hash<std::string>{}(path.string()); }
};
}
