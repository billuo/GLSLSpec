#pragma once

#include "glm/ivec2.hpp"
#include "glm/ivec4.hpp"

#include <unordered_map>
#include <memory>


struct GLFWwindow;

using Handle = GLFWwindow*;

struct Window {
    Window();
    Window(const Window&) = delete;

    Window(Window&& obj) noexcept : m_handle(obj.m_handle), m_callbacks(obj.m_callbacks)
    {
        obj.m_handle = nullptr;
        m_callbacks.register_all(m_handle);
    }

    Window& operator=(Window&& obj) noexcept
    {
        Window other(std::move(obj));
        swap(other);
    }

    void swap(Window& other) noexcept
    {
        std::swap(m_handle, other.m_handle);
        std::swap(m_callbacks, other.m_callbacks);
    }

    ~Window();

    void close();

    bool closed() const;

    void resize(int w, int h);

    void next_frame();

    std::string default_title() const;

    const glm::ivec4& viewport() const;
    const glm::ivec2& dimension() const;
    const glm::ivec2& dimension_frame() const;
    uint32_t FPS() const;

    double frame_delay()
    { return m_properties.frame_delay; }

    static Window* find_by_handle(Handle handle);
  private:
    static std::unordered_map<Handle, Window*> Instances;

    Handle m_handle = nullptr;

    struct Callbacks {
        static void default_on_window_size(Handle, int, int);
        static void default_on_key(Handle, int, int, int, int);
        static void default_on_mouse_button(Handle, int, int, int);
        static void default_on_scroll(Handle, double, double);
        static void default_on_cursor_pos(Handle, double, double);

        void (* on_window_size)(Handle, int, int) = default_on_window_size;
        void (* on_key)(Handle, int, int, int, int) = default_on_key;
        void (* on_mouse_button)(Handle, int, int, int) = default_on_mouse_button;
        void (* on_scroll)(Handle, double, double) = default_on_scroll;
        void (* on_cursor_pos)(Handle, double, double) = default_on_cursor_pos;

        void register_all(Handle);
    } m_callbacks;

    struct Properties {
        /// Position and dimension of viewport
        glm::ivec4 viewport = {0, 0, 0, 0};
        /// Dimension of the window
        glm::ivec2 dimension = {0, 0};
        /// Dimension of the frame buffer related with viewport
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
