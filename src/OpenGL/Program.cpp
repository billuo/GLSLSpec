#include "Debug.hpp"
#include "OpenGL/Introspection.hpp"
#include <algorithm>
#include <fstream>
#include <map>


namespace OpenGL {

decltype(Program::Pool) Program::Pool;

Program&
Program::attach(const Shader* shader)
{
    auto it = std::find(m_attached_shaders.begin(),
                        m_attached_shaders.end(),
                        shader);
    if (it != m_attached_shaders.end()) {
        DEBUG("Shader<name=%u> already attached to Program<name=%u>.",
              static_cast<GLuint>(shader->name()),
              static_cast<GLuint>(m_name));
    } else {
        m_attached_shaders.push_back(shader);
    }
    return *this;
}

Program&
Program::attach(const std::vector<const Shader*>& shaders)
{
    m_attached_shaders.insert(m_attached_shaders.end(),
                              shaders.begin(),
                              shaders.end());
    return *this;
}

Program&
Program::link()
{
    for (auto&& p : m_attached_shaders) {
        glAttachShader(static_cast<GLuint>(m_name), p->name());
        // assert(p->name() != 0);
        // GLint source_length;
        // glGetShaderiv(p->name(), GL_SHADER_SOURCE_LENGTH, &source_length);
        // assert(source_length > 0);
        // auto source_buffer = std::make_unique<GLchar[]>(source_length);
        // glGetShaderSource(p->name(), source_length, nullptr, source_buffer.get());
        // DEBUG("### Shader source ###\n%s", source_buffer.get());
    }
    glLinkProgram(static_cast<GLuint>(m_name));
    if (aux_get(GL_LINK_STATUS) == GL_FALSE) {
        DEBUG("Shader program linking failed:\n%s", aux_getInfoLog().get());
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
