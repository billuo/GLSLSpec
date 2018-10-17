#ifndef COMMON_HPP_FEDV9M4C
#define COMMON_HPP_FEDV9M4C
#include <GL/glew.h>

#include <GL/freeglut.h> // must be after <GL/glew.h>

/// OpenGL C++ wrapper
namespace OpenGL {

static inline GLint GetIneger(GLenum param) {
    GLint ret;
    glGetIntegerv(param, &ret);
    return ret;
}

GLsizei TypeSize(GLenum type);
const char* TypeString(GLenum type);

// const char* InterfaceString(GLenum interface);

} // namespace OpenGL

void MyDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
                            const GLchar* message, const void* user);

#endif /* end of include guard: COMMON_HPP_FEDV9M4C */
