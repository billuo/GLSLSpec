/**
 * @File Buffer.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Object/Buffer.hpp>


namespace OpenGL {

GLint
Buffer::get(GLenum param) const
{
    GLint ret = -1;
    glGetBufferParameteriv(name(), param, &ret);
    return ret;
}

} // namespace OpenGL
