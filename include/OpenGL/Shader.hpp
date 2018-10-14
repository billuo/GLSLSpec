/**
 * @file OpenGL/Shader.hpp
 * @brief Encapsulates OpenGL shader objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef SHADER_HPP_HWFB70NR
#define SHADER_HPP_HWFB70NR
#pragma once

#include "OpenGL/Object.hpp"
#include <string>

namespace OpenGL {

/**
 * @class Shader
 * @brief Operations on OpenGL shader object.
 */
class Shader : public Object<Shader> {
public:
    /// Convert shader type enum to const string.
    static const char* NameOfType(GLenum type);

public:
    Shader() : Object<Shader>() {}
    ~Shader() { Delete(); }

    /// Create this shader object
    void Create(GLenum type) {
        if (aux_CheckInitialized(false)) {
            m_name = glCreateShader(type);
        }
    }
    /// Delete this shader object
    void Delete() {
        if (aux_CheckInitialized(true)) {
            glDeleteShader(m_name);
        }
    }

    /// Add array of strings as sources of this shader
    void Source(const GLchar** sources, size_t count);

    /// Add a single string as the source of this shader
    void Source(const GLchar* source) { Source(&source, 1); }

    /// Compile this shader object.
    void Compile();

private:
    Shader(const Shader&);
    Shader& operator=(const Shader&);

    /// Query a parameter of this shader object
    GLint aux_GetParameter(GLenum param) const;

    /// Retrive information log in a smart pointer
    std::string GetInfoLog() const;
};

} // namespace OpenGL

#endif /* end of include guard: SHADER_HPP_HWFB70NR */
