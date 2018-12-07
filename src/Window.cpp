#include "Window.hpp"
#include "Options.hpp"
#include "OpenGL/Common.hpp"
#include "Thread.hpp"

#include <ctime>
#include <Window.hpp>


std::unique_ptr<Window> main_window;

std::unordered_map<Handle, Window*> Window::Instances;

Window::Window()
{
    auto& dim = options.window.dimension;
    auto& title = options.application.name;
    handle = glfwCreateWindow(dim.x, dim.y, title.c_str(), nullptr, nullptr);
    if (handle == nullptr) {
        Log::c("GLFW failed to create window");
        std::exit(EXIT_FAILURE);
    }
    Instances[handle] = this;
    glfwMakeContextCurrent(handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // XXX it needs a current context
    callbacks.register_all(handle);
}

Window::~Window()
{
    if (handle) {
        size_t n = Instances.erase(handle);
        if (n == 0) {
            Log::e("Window double deleted");
        } else if (n > 1) {
            Log::e("Window double inserted");
        }
        glfwDestroyWindow(handle);
        handle = nullptr;
    }
}

void
Window::Callbacks::default_on_window_size(GLFWwindow* handle, int w, int h)
{
    auto* window = find_by_handle(handle);
    window->resize(w, h);
}

void
Window::Callbacks::default_on_key(GLFWwindow*, int, int, int, int)
{

}

void
Window::Callbacks::default_on_mouse_button(GLFWwindow*, int, int, int)
{

}

void
Window::Callbacks::default_on_scroll(GLFWwindow*, double, double)
{

}

void
Window::Callbacks::default_on_cursor_pos(GLFWwindow*, double, double)
{

}

void
Window::Callbacks::register_all(GLFWwindow* handle)
{
    glfwSetWindowSizeCallback(handle, on_window_size);
    glfwSetKeyCallback(handle, on_key);
    glfwSetMouseButtonCallback(handle, on_mouse_button);
    glfwSetScrollCallback(handle, on_scroll);
    glfwSetCursorPosCallback(handle, on_cursor_pos);
}

Window*
Window::find_by_handle(GLFWwindow* handle)
{
    auto it = Instances.find(handle);
    if (it != Instances.end()) {
        return it->second;
    }
    Log::w("Window not found by handle {0:p}!", static_cast<void*>(handle));
    return nullptr;
}

void
Window::resize(int w, int h)
{
    properties.viewport = glm::ivec4(0, 0, w, h);
    auto& dim = properties.dimension;
    glfwGetWindowSize(handle, &dim.x, &dim.y);
    auto& dim_frame = properties.dimension_frame;
    glfwGetFramebufferSize(handle, &dim_frame.x, &dim_frame.y);
    glfwMakeContextCurrent(handle);
    glViewport(0, 0, dim_frame.x, dim_frame.y);
}

void
Window::next_frame()
{
    glfwSwapBuffers(handle);
    properties.frame_count++;
    properties.frame_delay = glfwGetTime() - properties.start_last_frame;
    properties.start_last_frame += properties.frame_delay; // set to this frame
    // Fix FPS if necessary
    if (!options.window.full_fps) {
        double leisure = options.window.frame_delay - frame_delay();
        if (leisure > 0) {
            Log::d("Sleeping for {} ms.", leisure * 1000);
            sleep_for_sec(static_cast<float>(leisure));
        }
    }
    // Measure performance by FPS
    properties.since_FPS += frame_delay();
    properties.FPS++;
    if (properties.since_FPS >= 1.0) {
        std::string&& title = default_title();
        glfwSetWindowTitle(handle, title.c_str());
        properties.FPS = 0;
        properties.since_FPS = 0.0;
    }
    glfwPollEvents();
}

std::string
Window::default_title() const
{
    return options.application.name +
           "(FPS=" +
           std::to_string(properties.FPS) +
           ")(Frame delay=" +
           std::to_string(1000 * properties.since_FPS / properties.FPS) +
           ')';
}

void
Window::close()
{ glfwSetWindowShouldClose(handle, GLFW_TRUE); }

bool
Window::closed() const
{ return glfwWindowShouldClose(handle) == GLFW_TRUE; }

const glm::ivec4&
Window::viewport() const
{ return properties.viewport; }

const glm::ivec2&
Window::dimension() const
{ return properties.dimension; }

const glm::ivec2&
Window::dimension_frame() const
{ return properties.dimension_frame; }

uint32_t
Window::FPS() const
{ return properties.FPS; }
