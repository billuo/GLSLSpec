/**
 * @file main.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Console.hpp>
#include <Sandbox.hpp>
#include <Window.hpp>
#include <csignal>

// TODO support simple texture
// TODO load texture from .obj
// TODO finish introspection

static decltype(std::this_thread::get_id()) main_thread_id;

void
OnExit()
{
    if (std::this_thread::get_id() != main_thread_id) {
        return;
    }
    options.flags.running = false;
    static std::atomic_bool exited = false;
    if (!exited) {
        exited = true;
        console->flush();
        sandbox.reset();
        main_window.reset();
        OpenGL::Exit();
    }
};

void
OnExitSignal(int)
{ OnExit(); }

int
main(int argc, char** argv)
{
    main_thread_id = std::this_thread::get_id();
    console = std::make_unique<Console>(); // XXX enable logging first
    std::atexit(OnExit);
    std::at_quick_exit(OnExit);
    auto&& register_signal_handler = [](auto signal)
    {
        if (std::signal(signal, OnExitSignal) == SIG_ERR) {
            Log::e("Failed to register signal {}", signal);
        }
    };
    register_signal_handler(SIGHUP);
    register_signal_handler(SIGILL);
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
    Watcher watcher(options.input_files);
    sandbox = std::make_unique<Sandbox>(watcher);
    declare_commands();
    console->execute_all(options.initial_commands);
    // main loop
    while (1000 * glfwGetTime() < options.application.TTL && options.flags.running && !main_window->closed()) {
        main_window->next_frame();
        console->execute_all();
        console->flush();
        auto&& updated = watcher.updated();
        for (auto&& file : updated) {
            sandbox->import(file);
        }
        sandbox->render_background();
        sandbox->render();
        sandbox->render_postprocess();
        if (options.flags.debug_draw) {
            sandbox->render_debug();
        }
    }
    OnExit();
    std::exit(EXIT_SUCCESS);
}
