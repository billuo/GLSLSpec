/**
 * @file OpenGL/Shader.hpp
 * @brief Encapsulates OpenGL shader objects.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Object.hpp"


namespace OpenGL {

/// OpenGL shader object
class Shader;

using shared_shader = std::shared_ptr<Shader>;

class Shader : public Object {
  public:
    explicit Shader(GLenum type, const GLchar* label = nullptr) : Object(pool.get(type))
    {
        if (label) {
            Object::label(label, GL_SHADER);
        }
    }

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    ~Shader()
    { pool.put(std::move(m_name)); }

    /// Add array of strings as sources of this shader
    void source(const GLchar** sources, size_t count);

    /// Add a single string as the source of this shader
    void source(const GLchar* source)
    { this->source(&source, 1); }

    /// Compile this shader object.
    void compile();

    /// Query a parameter
    GLint get(GLenum param) const;

  private:
    static struct {
        static Name get(GLenum type)
        {
            GLuint name = glCreateShader(type);
            return Name(name);
        }

        static void put(Name&& name)
        { glDeleteShader(name.get()); }

    } pool;

    /// Retrieve information log in a smart pointer
    std::unique_ptr<char[]> aux_get_info_log() const;
};

} // namespace OpenGL

