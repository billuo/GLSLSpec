#include "Debug.hpp"

#include <GL/glu.h> /// must come after <GL/glut.h>

const char* GLErrorString(int error) { return (const char*)(gluErrorString(error)); }

static inline const char* SourceString(GLenum source) {
    switch (source) {
    case GL_DEBUG_SOURCE_API: return "API";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
    case GL_DEBUG_SOURCE_THIRD_PARTY: return "3rd Party";
    case GL_DEBUG_SOURCE_APPLICATION: return "Application";
    case GL_DEBUG_SOURCE_OTHER: return "Other";
    default: return "UNKNOWN";
    }
}

static inline const char* TypeString(GLenum type) {
    switch (type) {
    case GL_DEBUG_TYPE_ERROR: return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined Behaviour";
    case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
    case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
    case GL_DEBUG_TYPE_MARKER: return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP: return "Group in";
    case GL_DEBUG_TYPE_POP_GROUP: return "Group out";
    case GL_DEBUG_TYPE_OTHER: return "Other";
    default: return "UNKNOWN";
    }
}

static inline const char* ServerityString(GLenum serverity) {
    switch (serverity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
    case GL_DEBUG_SEVERITY_LOW: return "Low";
    case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
    case GL_DEBUG_SEVERITY_HIGH: return "High";
    default: return "UNKNOWN";
    }
}

void MyDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
                            const GLchar* message, const void* user) {
    static FILE* glerr = fopen("GL.log", "w");
    if (serverity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        printf("\tOpenGL debug message:\n"
               "source:   %-20s\n"
               "type:     %-20s\n"
               "serverity:%-20s id:%u\n"
               "message:  %.*s\n",
               SourceString(source), TypeString(type), ServerityString(serverity), id, length, message);
    }
    fprintf(glerr,
            "\tOpenGL debug message:\n"
            "source:   %-20s\n"
            "type:     %-20s\n"
            "serverity:%-20s id:%u\n"
            "message:  %.*s\n",
            SourceString(source), TypeString(type), ServerityString(serverity), id, length, message);
}
