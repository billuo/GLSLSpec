/**
 * @file main.cpp
 * @brief Program entry point.
 * @author Zhen Luo 461652354@qq.com
 */
#include "Console.hpp"
#include "Options.hpp"
#include "Window.hpp"
#include "OpenGL/Common.hpp"

// TODO load meshes from .obj
// TODO support simple texture
// TODO load texture from .obj
// TODO finish introspection

namespace {

void
on_exit()
{
    OpenGL::Exit();
}

} // namespace

int
main(int argc, char** argv)
{
    std::atexit(on_exit);
    try {
        parse_options(argc - 1, argv + 1);
    } catch (invalid_option& e) {
        Log::e("{}", e.what());
        std::exit(EXIT_FAILURE);
    }
    if (options.flags.display_help) {
        print_usage(argv[0]);
        std::exit(EXIT_SUCCESS);
    }
    Watcher watcher(options.input_files);
    // declare_commands();
    OpenGL::Initialize();
    glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
    console = std::make_unique<Console>(options.initial_commands);
    while (options.flags.running && !main_window->closed()) {
        console->execute_all();
        main_window->next_frame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
