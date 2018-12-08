#include "Enumeration.hpp"
#include "FileSystem.hpp"
#include "Log.hpp"
#include "Options.hpp"

#include <fstream>
#include <iterator>
#include <algorithm>


namespace FS {

namespace {

inline void
print_fs_error(FS::details::filesystem_error& e)
{ Log::e("{}", e.what()); }

inline void
print_bad_alloc(std::bad_alloc& e)
{ Log::e("{}", e.what()); }

/// Resolve the canonical path of a relative path.
/// @param relative_path The path relative to the @p base_path.
/// @param base_path The base directory.
/// @details When determining the actual base path, @p base_path is considered first of all.
/// Only if not found under @p base_path will folders in @p include_folders be tried one by one.
/// @returns The canonical path if resolved successfully. Otherwise empty path.
std::string
resolve_url(const FS::path& base_path, const FS::path& relative_path)
{
    FS::path url;
    url = base_path / relative_path;
    if (details::exists(url)) {
        return details::canonical(url);
    }
    for (const auto& folder : options.includes) {
        url = folder / relative_path;
        if (details::exists(url)) {
            return details::canonical(url);
        }
    }
    return {};
}

/// Extract dependency from "#include <file>" or "#pragma include <file>".
/// @param line A single line of GLSL source code.
/// @return The relative path to the file included. Empty if @p line does not "include" anything.
/// @details This function does not check if the file exists or even is a valid path.
FS::path
extract_include(const std::string& line)
{
    auto contains = [](const std::string& str, const std::string& substr)
    { return str.find(substr) != std::string::npos; };
    if (contains(line, "#include ") || contains(line, "#pragma include")) {
        auto begin = line.find_first_of('"') + 1;
        auto len = line.find_last_of('"') - begin;
        FS::path ret = line.substr(begin, len);
        return ret;
    }
    return {};
}

} // namespace

ModifiedTimePoint
last_write_time(const FS::path& path) noexcept
{
    try {
        return details::last_write_time(path);
    } catch (details::filesystem_error& e) {
        print_fs_error(e);
    } catch (std::bad_alloc& e) {
        print_bad_alloc(e);
    }
    return {};
}

bool
exists(const std::string& name) noexcept
{
    try {
        return details::exists(name);
    } catch (details::filesystem_error& e) {
        print_fs_error(e);
    } catch (std::bad_alloc& e) {
        print_bad_alloc(e);
    }
    return false;
}

FS::path
canonical(const std::string& path) noexcept
{
    try {
        return details::canonical(path);
    } catch (details::filesystem_error& e) {
        print_fs_error(e);
    } catch (std::bad_alloc& e) {
        print_bad_alloc(e);
    }
    return {};
}

bool
load_shader(const FS::path& file_name, std::string& into, FS::FileList& dependencies)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(file_name);
    } catch (std::ifstream::failure& e) {
        Log::e("Failed to open file {}: {}", file_name, e.what());
        return false;
    }
    file.exceptions(std::ifstream::badbit);
    // parse for dependencies line by line.
    const auto&& base_path = file_name.parent_path();
    if (std::find(dependencies.begin(), dependencies.end(), file_name) != dependencies.end()) {
        Log::w("{} is already included! Cyclic include is skipped.", file_name);
        return true;
    }
    dependencies.push_back(file_name);
    std::string line;
    while (getline(file, line)) {
        auto&& include_file = extract_include(line);
        if (include_file.empty()) {
            // normal line, just append
            into += line + '\n';
        } else {
            // #include line, try to insert the content of the included file right here
            auto&& dependency = resolve_url(base_path, include_file);
            if (dependency.empty()) {
                Log::e("Can not resolve file: {}\nincluded by: {}", include_file, file_name);
                return false;
            }
            if (!load_shader(dependency, into, dependencies)) {
                // failed to load; log error
                std::string folders;
                for (const auto& folder : options.includes) {
                    folders += "\n\t";
                    folders += folder;
                }
                if (folders.empty()) {
                    folders = "\n\t<none>";
                }
                Log::e("Failed to load {}\nAddtional include folders:{}", dependency, folders);
                return false;
            }
        }
    }
    return true;
}

} // namespace FS
