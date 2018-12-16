/**
 * @File Options.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <glm/fwd.hpp>
#include <glm/ivec2.hpp>
#include <Utility/Log.hpp>
#include <FileSystem.hpp>
#include <Watcher.hpp>

#include <string>
#include <list>
#include <queue>
#include <set>


struct GlobalOptions {
    GlobalOptions();

    /// Application information
    struct Application {
        static constexpr bool debug = DEBUG_BUILD;
        const std::string name;
        const std::string version;
        const std::string build;
        const std::string acknowledgement;
        std::string prompt = ">> ";
        /// Time To Live of application in milliseconds.
        uint64_t TTL = std::numeric_limits<uint64_t>::max();
    } application;

    /// window specific options
    struct Window {
        /// Window position
        glm::ivec2 position = {0, 0};
        /// Window size
        glm::ivec2 dimension = {512, 512};
        /// Expected frame delay (inverse of expected FPS)
        double frame_delay = 1.0 / 60.0;
        /// Show cursor?
        bool cursor = false;
        /// Don't display window?
        bool hidden = false;
        /// Window should always stay on top of any others?
        bool always_on_top = false;
        /// Run at full FPS?
        bool full_fps = false;
    } window;

    /// OpenGL specific options
    struct OpenGL {
        struct Version {
            int major = OPENGL_MAJOR_VERSION;
            int minor = OPENGL_MINOR_VERSION;
        } version;
    } opengl;

    /// Various boolean flags
    struct Flags {
        /// Enable debug drawing?
        bool debug_draw = DEBUG_BUILD;
        /// Enable verbose output?
        bool verbose = false;
        /// Display help and immediately quit?
        bool display_help = false;
        /// quit after executing initial_commands?
        bool exit_after_execution = false;
        /// Should the application keep running?
        std::atomic_bool running = true;
    } flags;

    /// Commands to execute immediately after startup. Specified through command line options.
    std::queue<std::string> initial_commands;

    /// Files given directly.
    std::vector<DynamicFile> input_files;

    /// Files given through '-o'.
    std::vector<FS::path> output_files;

    /// Paths given through '-I'.
    std::vector<FS::path> includes;

    /// Additionally defined macros
    std::set<std::string> defines = {"GLSL_VIEWER=1"};

    /// Define a new macro. Will complain about duplicates.
    void define(std::string macro)
    { defines.emplace(std::move(macro)); }

    // TODO undefine and define with separated macro name and value
    /// Undefine a macro. Will complain about undefining non-existent ones.
    void undefine(const std::string& str)
    {
        auto it = defines.find(str);
        if (it == defines.end()) {
            Log::w("{} not yet defined", str);
        } else {
            defines.erase(it);
        }
    }

};

extern GlobalOptions options;

class invalid_option : public std::invalid_argument {
    using invalid_argument::invalid_argument;
};

void
print_usage(const std::string& name);

/// @brief Parse command line options and prepare global options.
/// @param argc Number of options in @p argv.
/// @param argv Array of options. Executable name should not be included!
/// @details Should be called before EVERYTHING in main(). It takes effect immediately upon
/// processing any options, except for those files that possible to load only after OpenGL
/// is initialized, which will be cached into options.file.to_loads.
void
parse_options(int argc, char* argv[]);

