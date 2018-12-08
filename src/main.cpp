/**
 * @file main.cpp
 * @brief Program entry point.
 * @author Zhen Luo 461652354@qq.com
 */
#include "Console.hpp"
#include "Sandbox.hpp"
#include "Options.hpp"
#include "Window.hpp"
#include "OpenGL/Common.hpp"
#include "OpenGL/Utility/Debug.hpp"

// TODO load meshes from .obj
// TODO support simple texture
// TODO load texture from .obj
// TODO finish introspection

int
main(int argc, char** argv)
{
    console = std::make_unique<Console>(); // XXX enable logging first
    std::atexit([]()
                {
                    sandbox.reset();
                    OpenGL::Exit();
                });
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
    // prepare everything
    OpenGL::Initialize();
    sandbox = std::make_unique<Sandbox>();
    Watcher watcher(options.input_files);
    watcher.set_callback([&watcher](const DynamicFile& file)
                         {
                             if (sandbox) {
                                 sandbox->on_update(file);
                             }
                         });
    declare_commands();
    console->execute_all(options.initial_commands);
    while (options.flags.running && !main_window->closed()) {
        main_window->next_frame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        console->execute_all();
        console->flush();
        sandbox->update();
        sandbox->render();
    }
    std::exit(EXIT_SUCCESS);
}
