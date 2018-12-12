#include <OpenGL/Common.hpp>
#include <OpenGL/Debug.hpp>
#include <Math/Math.hpp>
#include <Utility/Log.hpp>
#include <Options.hpp>
#include <Window.hpp>


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
    glfwWindowHint(GLFW_VISIBLE, options.window.hidden ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, options.window.always_on_top ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, DEBUG_BUILD ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options.opengl.version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options.opengl.version.minor);
    main_window = std::make_unique<Window>();
    glDebugMessageCallback(OpenGLOnDebug, nullptr);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void
Exit()
{
    main_window.reset();
}

} // namespace OpenGL
