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
    m_handle = glfwCreateWindow(dim.x, dim.y, title.c_str(), nullptr, nullptr);
    if (m_handle == nullptr) {
        Log::c("GLFW failed to create window");
        std::exit(EXIT_FAILURE);
    }
    Instances[m_handle] = this;
    glfwMakeContextCurrent(m_handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // XXX it needs a current context
    m_callbacks.register_all(m_handle);
}

Window::~Window()
{
    if (m_handle) {
        size_t n = Instances.erase(m_handle);
        if (n == 0) {
            Log::e("Window double deleted");
        } else if (n > 1) {
            Log::e("Window double inserted");
        }
        glfwDestroyWindow(m_handle);
        m_handle = nullptr;
    }
}

void
Window::Callbacks::default_on_window_size(GLFWwindow* handle, int w, int h)
{
    auto* window = find_by_handle(handle);
    window->resize(w, h);
}

void
Window::Callbacks::default_on_key(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    Log::d("Default on key: pressed '{}'", static_cast<char>(key));
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        if (auto window = find_by_handle(handle)) {
            window->close();
        }
    }
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
    auto& dim = m_properties.dimension;
    auto& dim_frame = m_properties.dimension_frame;
    glfwGetWindowSize(m_handle, &dim.x, &dim.y);
    glfwGetFramebufferSize(m_handle, &dim_frame.x, &dim_frame.y);
    set_viewport(dim_frame);
}

void
Window::set_viewport(glm::ivec2 size)
{
    glfwMakeContextCurrent(m_handle);
    glViewport(0, 0, size.x, size.y);
    m_properties.viewport = glm::ivec4(0, 0, size.x, size.y);
}

void
Window::next_frame()
{
    glfwSwapBuffers(m_handle);
    m_properties.frame_count++;
    m_properties.frame_delay = glfwGetTime() - m_properties.start_last_frame;
    m_properties.start_last_frame += m_properties.frame_delay; // set to this frame
    // Fix FPS if necessary
    if (!options.window.full_fps) {
        double leisure = options.window.frame_delay - frame_delay();
        if (leisure > 0) {
            sleep_for_sec(static_cast<float>(leisure));
        }
    }
    // Measure performance by FPS
    m_properties.since_FPS += frame_delay();
    m_properties.FPS++;
    if (m_properties.since_FPS >= 1.0) {
        std::string&& title = default_title();
        glfwSetWindowTitle(m_handle, title.c_str());
        m_properties.FPS = 0;
        m_properties.since_FPS = 0.0;
    }
    glfwPollEvents();
}

std::string
Window::default_title() const
{
    return options.application.name +
           "(FPS=" +
           std::to_string(m_properties.FPS) +
           ")(Frame delay=" +
           std::to_string(1000 * m_properties.since_FPS / m_properties.FPS) +
           ')';
}

void
Window::close()
{ glfwSetWindowShouldClose(m_handle, GLFW_TRUE); }

bool
Window::closed() const
{ return glfwWindowShouldClose(m_handle) == GLFW_TRUE; }

const glm::ivec4&
Window::viewport() const
{ return m_properties.viewport; }

const glm::ivec2&
Window::dimension() const
{ return m_properties.dimension; }

const glm::ivec2&
Window::dimension_frame() const
{ return m_properties.dimension_frame; }

uint32_t
Window::FPS() const
{ return m_properties.FPS; }