#include "OpenGL/Debug.hpp"
#include "Log.hpp"
#include "Debug.hpp"


namespace {

inline const char*
SourceString(GLenum source)
{
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "Shader Compiler";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "Window System";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "3rd Party";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "Application";
        case GL_DEBUG_SOURCE_OTHER:
            return "Other";
        default:
            return "UNKNOWN";
    }
}

inline const char*
TypeString(GLenum type)
{
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "Deprecated";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "Undefined Behaviour";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "Performance";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "Portability";
        case GL_DEBUG_TYPE_MARKER:
            return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "Group in";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "Group out";
        case GL_DEBUG_TYPE_OTHER:
            return "Other";
        default:
            return "UNKNOWN";
    }
}

} // namespace

namespace OpenGL {

const char*
GLErrorString(uint32_t error)
{ return (const char*) (gluErrorString(error)); }

void
OpenGLOnDebug(GLenum source, GLenum type, GLuint id, GLenum severity,
              GLsizei length, const GLchar* message, const void* user)
{
    static const char* const fmt = "[OpenGL::{}+{}+] [id{}] {}";
    switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            Log::d(fmt, SourceString(source), TypeString(type), id, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log::i(fmt, SourceString(source), TypeString(type), id, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log::w(fmt, SourceString(source), TypeString(type), id, message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            Log::e(fmt, SourceString(source), TypeString(type), id, message);
            break;
        default:
            UNREACHABLE;
    }
}

} // namespace OpenGL
