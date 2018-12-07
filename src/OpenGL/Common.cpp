#include "OpenGL/Common.hpp"
#include "OpenGL/Utility/Debug.hpp"
#include "Math.hpp"
#include "Log.hpp"
#include "Options.hpp"
#include "Window.hpp"


namespace OpenGL {

void
Initialize()
{
    glfwSetErrorCallback([](int err, const char* msg)
                         { Log::e("GLFW error[{0:x}]: {}", err, msg); });
    if (glfwInit() == GLFW_FALSE) {
        Log::c("GLFW failed to initialize!!!");
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_VISIBLE, options.window.hidden ? GL_FALSE : GL_TRUE);
    glfwWindowHint(GLFW_FLOATING, options.window.always_on_top ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, DEBUG_BUILD ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options.opengl.version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options.opengl.version.minor);
    main_window = std::make_unique<Window>();
    // sandbox = std::make_unique<Sandbox>();
    glDebugMessageCallback(OpenGLOnDebug, nullptr);
}

void
Exit()
{
    main_window.reset();
}

} // namespace OpenGL
