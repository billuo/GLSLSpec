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
    glfwWindowHint(GLFW_RESIZABLE, options.flags.resizable ? GLFW_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_VISIBLE, options.window.hidden ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, options.window.always_on_top ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, DEBUG_BUILD ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options.opengl.version.major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options.opengl.version.minor);
    main_window = std::make_unique<Window>();
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLOnDebug, nullptr);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); // for background rendering
}

void
Exit()
{
    glfwTerminate();
}

} // namespace OpenGL
