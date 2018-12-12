#ifndef OPENGL_LAB_DEBUG_HPP
#define OPENGL_LAB_DEBUG_HPP

#include <OpenGL/Common.hpp>


#define OPENGL_TRACE if(auto err = glGetError()) {Log::d("error[{}], line#{}\nfile:{}", err, __LINE__, __FILE__);}

namespace OpenGL {

void
OpenGLOnDebug(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
              const GLchar* message, const void* user);

} // namespace OpenGL

#endif //OPENGL_LAB_DEBUG_HPP
