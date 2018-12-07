#ifndef OPENGL_LAB_DEBUG_HPP
#define OPENGL_LAB_DEBUG_HPP

#include "OpenGL/Common.hpp"


namespace OpenGL {

void
OpenGLOnDebug(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
              const GLchar* message, const void* user);

} // namespace OpenGL

#endif //OPENGL_LAB_DEBUG_HPP
