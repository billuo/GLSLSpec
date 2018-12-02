#ifndef OPENGL_LAB_DEBUG_HPP
#define OPENGL_LAB_DEBUG_HPP

#include "OpenGL/Common.hpp"


namespace OpenGL {

/// Convert OpenGL error code to error string.
const char*
GLErrorString(uint32_t error);

void
OpenGLOnDebug(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
              const GLchar* message, const void* user);

} // namespace OpenGL

#endif //OPENGL_LAB_DEBUG_HPP
