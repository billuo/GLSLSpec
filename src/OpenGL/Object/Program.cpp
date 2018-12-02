#include "Log.hpp"
#include "OpenGL/Introspection/Introspection.hpp"
#include <algorithm>


namespace OpenGL {

decltype(Program::pool) Program::pool;

Program&
Program::attach(shared_shader shader)
{
    auto it = std::find(m_attached_shaders.begin(),
                        m_attached_shaders.end(),
                        shader);
    if (it != m_attached_shaders.end()) {
        Log::w("Shader<name={}> already attached to Program<name={}>.",
               shader->name(),
               m_name);
    } else {
        m_attached_shaders.insert(shader);
    }
    return *this;
}

Program&
Program::attach(std::initializer_list<shared_shader> shaders)
{
    for (auto&& ss : shaders) {
        attach(ss);
    }
    m_attached_shaders.insert(shaders);
    return *this;
}

Program&
Program::link()
{
    for (auto&& p : m_attached_shaders) {
        glAttachShader(static_cast<GLuint>(m_name), p->name());
    }
    glLinkProgram(static_cast<GLuint>(m_name));
    if (aux_get(GL_LINK_STATUS) == GL_FALSE) {
        Log::e("Shader program linking failed:\n{}", aux_getInfoLog().get());
    }
    return *this;
}

std::unique_ptr<GLchar[]>
Program::aux_getInfoLog() const
{
    std::unique_ptr<GLchar[]> ret;
    GLint length = aux_get(GL_INFO_LOG_LENGTH);
    assert(length >= 0);
    ret = std::make_unique<GLchar[]>(length);
    glGetProgramInfoLog(static_cast<GLuint>(m_name),
                        length,
                        nullptr,
                        ret.get());
    return ret;
}

GLint
Program::aux_get(GLenum pname) const
{
    GLint result = -1;
    glGetProgramiv(static_cast<GLuint>(m_name), pname, &result);
    return result;
}

GLint
Program::aux_getStage(GLenum stage, GLenum pname) const
{
    GLint result = -1;
    glGetProgramStageiv(static_cast<GLuint>(m_name), stage, pname, &result);
    return result;
}
} // namespace OpenGL
