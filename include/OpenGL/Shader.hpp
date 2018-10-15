/**
 * @file OpenGL/Shader.hpp
 * @brief Encapsulates OpenGL shader objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef SHADER_HPP_HWFB70NR
#define SHADER_HPP_HWFB70NR
#pragma once

#include "OpenGL/Object.hpp"
#include <memory>
#include <string>

namespace OpenGL {

/**
 * @class Shader
 * @brief Operations on OpenGL shader object.
 */
class Shader : public Object<Shader> {
public:
    Shader() = default;
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;
    ~Shader() {
        if (Initialized()) {
            Delete();
        }
    }

    /// Create this shader object
    void Create(GLenum type) {
        if (aux_CheckInitialized(false)) {
            SetName(glCreateShader(type));
        }
    }
    /// Delete this shader object
    void Delete() {
        if (aux_CheckInitialized(true)) {
            glDeleteShader(Name());
        }
    }

    /// Add array of strings as sources of this shader
    void Source(const GLchar** sources, size_t count);
    /// Add a single string as the source of this shader
    void Source(const GLchar* source) { Source(&source, 1); }

    /// Compile this shader object.
    void Compile();

private:
    /// Query a parameter of this shader object
    GLint aux_GetParameter(GLenum param) const;

    /// Retrive information log in a smart pointer
    std::unique_ptr<char[]> aux_GetInfoLog() const;
};

namespace ShaderResource {

static const GLenum GL_UNKNOWN_SHADER = 0;

Shader& GetShader(const std::string& dir, const std::string& source, GLenum type = GL_UNKNOWN_SHADER,
                  bool force_compile = false);

} // namespace ShaderResource

} // namespace OpenGL

#endif /* end of include guard: SHADER_HPP_HWFB70NR */
