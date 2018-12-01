#include "Debug.hpp"
#include "OpenGL/Common.hpp"


namespace {

inline const char* SourceString(GLenum source)
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

inline const char* TypeString(GLenum type)
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

inline const char* SeverityString(GLenum severity)
{
    switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "Notification";
        case GL_DEBUG_SEVERITY_LOW:
            return "Low";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "Medium";
        case GL_DEBUG_SEVERITY_HIGH:
            return "High";
        default:
            return "UNKNOWN";
    }
}

const size_t BufferSize = 1024;
GLchar Buffer[BufferSize];

}

const char* GLErrorString(uint32_t error)
{ return (const char*) (gluErrorString(error)); }

void
myDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                       GLsizei length, const GLchar* message, const void* user)
{
    snprintf(Buffer,
             BufferSize,
             "\tOpenGL debug message:\n"
             "source:   %-20s\n"
             "type:     %-20s\n"
             "severity: %-20s id:%u\n"
             "message:  %.*s\n",
             SourceString(source),
             TypeString(type),
             SeverityString(severity),
             id,
             length,
             message);
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        printf("%s", Buffer);
    }
    static FILE* gl_error_log = fopen("GL.log", "w");
    if (gl_error_log) {
        fprintf(gl_error_log, "%s", Buffer);
    }
}
