#include "Debug.hpp"
#include "OpenGL/Introspection.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <map>
#include <utility>

namespace OpenGL {

void Program::Attach(const Shader* shader) {
    auto it = std::find(m_attached_shaders.begin(), m_attached_shaders.end(), shader);
    if (it != m_attached_shaders.end()) {
        DEBUG("Shader<name=%u> already attached to Program<name=%u>.", static_cast<GLuint>(shader->m_name),
              static_cast<GLuint>(m_name));
    } else {
        m_attached_shaders.push_back(shader);
    }
}

void Program::Attach(const std::vector<const Shader*>& shaders) {
    m_attached_shaders.insert(m_attached_shaders.end(), shaders.begin(), shaders.end());
}

void Program::Link() {
    for (auto&& p : m_attached_shaders) {
        glAttachShader(m_name, p->m_name);
    }
    glLinkProgram(m_name);
    if (aux_Get(GL_LINK_STATUS) == GL_FALSE) {
        DEBUG("Shader program linking failed:\n%s", aux_GetInfoLog().get());
    }
}

// const Program::UniformBlock* Program::GetUniformBlock(const GLchar* block_name) {
//     //
//     // look up cached uniform blocks
//     auto it = std::find_if(m_uniform_blocks.begin(), m_uniform_blocks.end(), [block_name](const UniformBlock& b) {
//         static std::string str_name(block_name);
//         return b.name.get() == str_name;
//     });
//     if (it != m_uniform_blocks.end()) {
//         return std::addressof(*it);
//     }
//     UniformBlock ret;
//     //
//     // get index and size of uniform block
//     GLint n_uniforms;
//     ret.index = glGetUniformBlockIndex(m_name, block_name);
//     if (ret.index == GL_INVALID_INDEX) {
//         glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_LOW, -1,
//                              "Uniform block not found");
//         return nullptr;
//     }
//     glGetActiveUniformBlockiv(m_name, ret.index, GL_UNIFORM_BLOCK_DATA_SIZE, &(ret.size));
//     glGetActiveUniformBlockiv(m_name, ret.index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &n_uniforms);
//     int len = strlen(block_name);
//     ret.name = std::make_unique<GLchar[]>(len);
//     strcpy(ret.name.get(), block_name);
//     ret.uniforms.resize(n_uniforms);
//     //
//     // get indices of uniforms
//     auto&& uniform_indices = std::make_unique<GLint[]>(n_uniforms);
//     // XXX glGetProgramResourceiv return -1 for GL_LOCATION of uniforms in uniform block.
//     glGetActiveUniformBlockiv(m_name, ret.index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniform_indices.get());
//     for (GLint i = 0; i < n_uniforms; ++i) {
//         ret.uniforms[i].first.index = uniform_indices[i];
//     }
//     //
//     // get name, type and offset of uniforms
//     const GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_OFFSET };
//     GLint results[countof(properties)];
//     const size_t N = countof(properties);
//     for (GLint i = 0; i < n_uniforms; ++i) {
//         glGetProgramResourceiv(m_name, GL_UNIFORM, i, N, properties, N, nullptr, results);
//         auto&& name = std::make_unique<GLchar[]>(results[0] + 1); // XXX one more unnecessary byte, just in case
//         glGetProgramResourceName(m_name, GL_UNIFORM, i, results[0] + 1, nullptr, name.get());
//         ret.uniforms[i].first.type = results[1];
//         ret.uniforms[i].second = results[2];
//         ret.uniforms[i].first.name = std::move(name);
//     }
//     //
//     // cache and return
//     m_uniform_blocks.emplace_back(std::move(ret));
//     return &m_uniform_blocks.back();
// }

std::unique_ptr<GLchar[]> Program::aux_GetInfoLog() const {
    std::unique_ptr<GLchar[]> ret;
    GLint length = aux_Get(GL_INFO_LOG_LENGTH);
    assert(length >= 0);
    ret = std::make_unique<GLchar[]>(length);
    glGetProgramInfoLog(m_name, length, nullptr, ret.get());
    return ret;
}

GLint Program::aux_Get(GLenum pname) const {
    GLint result = -1;
    glGetProgramiv(m_name, pname, &result);
    return result;
}

GLint Program::aux_GetStage(GLenum stage, GLenum pname) const {
    GLint result = -1;
    glGetProgramStageiv(m_name, stage, pname, &result);
    return result;
}

} // namespace OpenGL
