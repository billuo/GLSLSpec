/**
 * @File Window.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Window.hpp>
#include <Options.hpp>
#include <Sandbox.hpp>
#include <OpenGL/Common.hpp>
#include <Utility/Debug.hpp>
#include <Utility/Thread.hpp>

#include <ctime>
#include <Window.hpp>
#include <Scene/Camera.hpp>


std::unique_ptr<Window> main_window;

std::unordered_map<Handle, Window*> Window::Instances;

Window::Window()
{
    m_properties.dimension = options.window.dimension;
    auto& dim = m_properties.dimension;
    auto& title = options.application.name;
    m_handle = glfwCreateWindow(dim.x, dim.y, title.c_str(), nullptr, nullptr);
    if (m_handle == nullptr) {
        Log::c("GLFW failed to create window");
        std::exit(EXIT_FAILURE);
    }
    Instances[m_handle] = this;
    glfwMakeContextCurrent(m_handle);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // XXX it needs a current context
    callbacks.register_all(m_handle);
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
    if (window) {
        window->resize(w, h);
    }
}

void
Window::Callbacks::default_on_key(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
    switch (key) {
        case GLFW_KEY_Q:
            if (action == GLFW_PRESS) {
                if (auto window = find_by_handle(handle)) {
                    window->close();
                }
            }
            break;
            // TODO time based movements
            //  - track key states
        case GLFW_KEY_W:
            if (action != GLFW_RELEASE) {
                sandbox->camera.dolly(-0.2f);
            }
            break;
        case GLFW_KEY_A:
            if (action != GLFW_RELEASE) {
                sandbox->camera.track(0.2f);
            }
            break;
        case GLFW_KEY_S:
            if (action != GLFW_RELEASE) {
                sandbox->camera.dolly(-0.2f);
            }
            break;
        case GLFW_KEY_D:
            if (action != GLFW_RELEASE) {
                sandbox->camera.track(0.2f);
            }
            break;
        case GLFW_KEY_P:
            if (action == GLFW_PRESS) {
                VALUE(sandbox->camera.axes());
            }
            break;
        default:
            break;
    }
}

void
Window::Callbacks::default_on_mouse_button(GLFWwindow* handle, int button, int action, int mods)
{
    auto window = find_by_handle(handle);
    if (window) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            window->callbacks.button_down.x = action == GLFW_PRESS;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            window->callbacks.button_down.y = action == GLFW_PRESS;
        }
    }
}

void
Window::Callbacks::default_on_scroll(GLFWwindow* handle, double dx, double dy)
{
    constexpr float factor = 1.1892; // = 2^0.25
    if (dy != 0.0) {
        float n = std::pow(factor, static_cast<float>(dy));
        sandbox->camera.distance(glm::vec3(0.0f), sandbox->camera.distance_to(glm::vec3(0.0f)) / n);
    }
}

void
Window::Callbacks::default_on_cursor_pos(GLFWwindow* handle, double xpos, double ypos)
{
    auto window = find_by_handle(handle);
    if (window) {
        auto& cb = window->callbacks;
        auto&& delta = glm::dvec2(xpos, ypos) - cb.last_cursor_pos;
        if (cb.last_button_down.x && cb.button_down.x) {
            cb.on_mouse_drag(window, GLFW_MOUSE_BUTTON_LEFT, delta.x, delta.y);
        }
        if (cb.last_button_down.y && cb.button_down.y) {
            cb.on_mouse_drag(window, GLFW_MOUSE_BUTTON_RIGHT, delta.x, delta.y);
        }
        cb.last_cursor_pos = glm::dvec2(xpos, ypos);
        cb.last_button_down = cb.button_down;
    }
}

void
Window::Callbacks::default_on_mouse_drag(Window* _this, int button, double dx, double dy)
{
    assert(_this);
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            sandbox->camera.orbit(Math::Degree::Of(static_cast<Math::Degree::value_type>(dy)),
                                  Math::Degree::Of(static_cast<Math::Degree::value_type>(-dx)),
                                  glm::vec3(0.0f));
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (dx != 0.0) {
                sandbox->camera.pan(180_deg / _this->size().x * dx);
            }
            if (dy != 0.0) {
                sandbox->camera.tilt(180_deg / _this->size().y * -dy);
            }
            break;
        default:
            break;
    }
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
    m_properties.dimension = glm::ivec2(w, h);
    auto& dim_frame = m_properties.dimension_frame;
    glfwGetFramebufferSize(m_handle, &dim_frame.x, &dim_frame.y);
    set_viewport(dim_frame);
}

void
Window::set_viewport(glm::ivec2 size)
{
    glfwMakeContextCurrent(m_handle);
    glViewport(0, 0, size.x, size.y);
    sandbox->camera.set_aspect(static_cast<float>(size.x) / size.y);
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

glm::ivec2
Window::size() const
{ return m_properties.dimension; }

glm::ivec2
Window::frame_buffer_size() const
{ return m_properties.dimension_frame; }

uint32_t
Window::FPS() const
{ return m_properties.FPS; }
