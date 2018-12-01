#ifndef OPENGL_LAB_COMMON_HPP
#define OPENGL_LAB_COMMON_HPP

#include <GL/glew.h>

#include <GL/freeglut.h> // must be after <GL/glew.h>

/// OpenGL C++ wrapper
namespace OpenGL {

static inline GLint
getInteger(GLenum param)
{
    GLint ret;
    glGetIntegerv(param, &ret);
    return ret;
}

GLsizei
sizeOfType(GLenum type);

const char*
nameOfType(GLenum type);

// const char* InterfaceString(GLenum interface);

} // namespace OpenGL

void
myDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity,
                       GLsizei length, const GLchar* message,
                       const void* user);

#endif /* end of include guard: OPENGL_LAB_COMMON_HPP */
