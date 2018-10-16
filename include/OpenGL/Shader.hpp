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
 * @brief Encapsulated OpenGL shader objects
 */
class Shader : public Object {
    friend class Program;
    using Base = Object;
    static Name Get(GLenum type) {
        GLuint name = glCreateShader(type);
        assert(name);
        return Name(name);
    }
    static void Put(Name&& name) { glDeleteShader(name.get()); }

public:
    static const GLenum GL_UNKNOWN_SHADER = 0;

    static Shader& CompileFrom(const std::string& dir, const std::string& source, GLenum type = GL_UNKNOWN_SHADER,
                               bool force_compile = false);

public:
    Shader(GLenum type) : Base(Get(type)) {}
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;
    ~Shader() { Put(std::move(m_name)); }

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

} // namespace OpenGL

#endif /* end of include guard: SHADER_HPP_HWFB70NR */
