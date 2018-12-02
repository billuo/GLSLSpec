/**
 * @file OpenGL/Shader.hpp
 * @brief Encapsulates OpenGL shader objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef SHADER_HPP_HWFB70NR
#define SHADER_HPP_HWFB70NR
#pragma once

#include "Object.hpp"


namespace OpenGL {

/// OpenGL shader object
class Shader;

using shared_shader = std::shared_ptr<Shader>;

class Shader : public Object {
  public:
    explicit Shader(GLenum type, const GLchar* label = nullptr) : Object(Get(
            type), label, GL_SHADER)
    {}

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    ~Shader()
    { Put(std::move(m_name)); }

    /// Add array of strings as sources of this shader
    void source(const GLchar** sources, size_t count);

    /// Add a single string as the source of this shader
    void source(const GLchar* source)
    { this->source(&source, 1); }

    /// Compile this shader object.
    void compile();

  private:
    static Name Get(GLenum type)
    {
        GLuint name = glCreateShader(type);
        if (name == 0) {
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION,
                                 GL_DEBUG_TYPE_ERROR,
                                 0,
                                 GL_DEBUG_SEVERITY_MEDIUM,
                                 -1,
                                 "Failed to create shader");
        }
        return Name(name);
    }

    static void Put(Name&& name)
    { glDeleteShader(name.get()); }

    /// Query a parameter of this shader object
    GLint aux_GetParameter(GLenum param) const;
    /// Retrieve information log in a smart pointer
    std::unique_ptr<char[]> aux_GetInfoLog() const;
};

} // namespace OpenGL

#endif /* end of include guard: SHADER_HPP_HWFB70NR */
