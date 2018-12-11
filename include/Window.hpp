#pragma once

#include "glm/ivec2.hpp"
#include "glm/ivec4.hpp"
#include "glm/bvec2.hpp"
#include "glm/dvec2.hpp"

#include <unordered_map>
#include <memory>


struct GLFWwindow;

using Handle = GLFWwindow*;

struct Window {
    Window();
    Window(const Window&) = delete;

    Window(Window&& obj) noexcept : m_handle(obj.m_handle), callbacks(obj.callbacks)
    {
        obj.m_handle = nullptr;
        callbacks.register_all(m_handle);
    }

    Window& operator=(Window&& obj) noexcept
    {
        Window other(std::move(obj));
        swap(other);
    }

    void swap(Window& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
        std::swap(callbacks, other.callbacks);
    }

    ~Window();

    void close();

    bool closed() const;

    void resize(int w, int h);

    void next_frame();

    std::string default_title() const;

    const glm::ivec4& viewport() const;
    glm::ivec2 size() const;
    glm::ivec2 frame_buffer_size() const;
    uint32_t FPS() const;

    double frame_delay()
    { return m_properties.frame_delay; }

    static Window* find_by_handle(Handle handle);

    struct Callbacks {
        static void default_on_window_size(Handle, int, int);
        static void default_on_key(Handle, int, int, int, int);
        static void default_on_mouse_button(Handle, int, int, int);
        static void default_on_scroll(Handle, double, double);
        static void default_on_cursor_pos(Handle, double, double);
        static void default_on_mouse_drag(Window*, int, double, double);

        void (* on_window_size)(Handle, int, int) = default_on_window_size;
        void (* on_key)(Handle, int, int, int, int) = default_on_key;
        void (* on_mouse_button)(Handle, int, int, int) = default_on_mouse_button;
        void (* on_scroll)(Handle, double, double) = default_on_scroll;
        void (* on_cursor_pos)(Handle, double, double) = default_on_cursor_pos;
        void (* on_mouse_drag)(Window*, int, double, double) = default_on_mouse_drag; // custom

        void register_all(Handle);

        /// left/right button is down?
        glm::bvec2 button_down = glm::bvec2(false, false);
        glm::dvec2 last_cursor_pos = glm::dvec2(0.0, 0.0);
        glm::bvec2 last_button_down = glm::bvec2(false, false);
    } callbacks;

  private:
    /// Map raw handle to instance of this class. May as well use glfwSet/GetWindowUserPointer?
    static std::unordered_map<Handle, Window*> Instances;

    Handle m_handle = nullptr;

    struct Properties {
        /// Position and dimension of viewport (in memory pixels)
        glm::ivec4 viewport = {0, 0, 0, 0};
        /// Dimension of the window (in screen coordinates)
        glm::ivec2 dimension = {0, 0};
        /// Dimension of the frame buffer for the whole window.
        glm::ivec2 dimension_frame = {0, 0};
        /// Number of frames rendered
        uint64_t frame_count = 0;
        /// Time when last frame started to render
        double start_last_frame = 0.0;
        /// Time when this frame started to render
        double frame_delay = 0.0;
        /// Time since FPS was last updated
        double since_FPS = 0.0;
        /// Number of frames rendered since FPS was last updated
        uint32_t FPS = 0;
    } m_properties;

    void set_viewport(glm::ivec2 size);
};

extern std::unique_ptr<Window> main_window;
