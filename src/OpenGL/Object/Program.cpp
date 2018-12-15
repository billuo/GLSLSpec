/**
 * @File Program.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Utility/Log.hpp>
#include <OpenGL/Introspection/Introspector.hpp>
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
        this->~Program();
    }
    return *this;
}

std::unique_ptr<GLchar[]>
Program::get_info_log() const
{
    GLint length = get(GL_INFO_LOG_LENGTH);
    if (length <= 0) {
        return {};
    }
    auto ret = std::make_unique<GLchar[]>(length);
    glGetProgramInfoLog(static_cast<GLuint>(m_name), length, nullptr, ret.get());
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

Program&
Program::set(GLenum param, GLint value)
{
    glProgramParameteri(name(), param, value);
    return *this;
}

Program::~Program()
{
    Introspector::Put(*this);
    pool().put(std::move(m_name));
}

Weak<Introspector>
Program::interfaces() const
{ return Introspector::Get(*this); }

} // namespace OpenGL
