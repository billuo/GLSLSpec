#include "Log.hpp"
#include "OpenGL/Introspection/Interface.hpp"
#include <algorithm>
#include <OpenGL/Object/Program.hpp>


namespace OpenGL {

Program&
Program::attach(const Shader& shader)
{
    glAttachShader(name(), shader.name());
    return *this;
}

Program&
Program::link()
{
    glLinkProgram(name());
    if (get(GL_LINK_STATUS) == GL_FALSE) {
        Log::e("Shader program linking failed:\n{}", get_info_log().get());
    }
    return *this;
}

std::unique_ptr<GLchar[]>
Program::get_info_log() const
{
    std::unique_ptr<GLchar[]> ret;
    GLint length = get(GL_INFO_LOG_LENGTH);
    assert(length >= 0);
    ret = std::make_unique<GLchar[]>(length);
    glGetProgramInfoLog(static_cast<GLuint>(m_name),
                        length,
                        nullptr,
                        ret.get());
    return ret;
}

GLint
Program::get(GLenum param) const
{
    GLint result = -1;
    glGetProgramiv(static_cast<GLuint>(m_name), param, &result);
    return result;
}

GLint
Program::get_stage(GLenum stage, GLenum pname) const
{
    GLint result = -1;
    glGetProgramStageiv(static_cast<GLuint>(m_name), stage, pname, &result);
    return result;
}

void
Program::set(GLenum param, GLint value)
{ glProgramParameteri(name(), param, value); }

} // namespace OpenGL
