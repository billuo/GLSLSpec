/**
 * @File Debug.hpp
 * @basic OpenGL specific debugging macros and routines should go here.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Common.hpp>


#define OPENGL_TRACE if(auto err = glGetError()) {Log::d("error[{}], line#{}\nfile:{}", err, __LINE__, __FILE__);}

namespace OpenGL {

void
OpenGLOnDebug(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
              const GLchar* message, const void* user);

} // namespace OpenGL

