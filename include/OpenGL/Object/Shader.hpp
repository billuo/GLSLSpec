/**
 * @file OpenGL/Shader.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Object.hpp>


namespace OpenGL {

/// OpenGL shader object
class Shader;

using shared_shader = std::shared_ptr<Shader>;

class Shader : public Object {
    static auto& pool()
    {
        static struct {
            Name get(GLenum type)
            {
                GLuint name = glCreateShader(type);
                return Name(name);
            }

            void put(Name name)
            { glDeleteShader(name.get()); }
        } singleton;
        return singleton;
    }

  public:
    explicit Shader(GLenum type) : Object(pool().get(type))
    {}

    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    ~Shader()
    { pool().put(std::move(m_name)); }

    using Object::label;

    Shader& label(const std::string& label)
    { return this->label(label.c_str()); }

    Shader& label(const GLchar* label)
    {
        Object::label(label, GL_SHADER);
        return *this;
    }

    /// Add array of strings as sources of this shader
    Shader& source(const GLchar** sources, size_t count);

    /// Add a single string as the source of this shader
    Shader& source(const std::string& source);

    /// Compile this shader object.
    Shader& compile();

    /// Query a parameter
    GLint get(GLenum param) const;

    /// Retrieve information log in a smart pointer
    std::unique_ptr<char[]> get_info_log() const;

};

} // namespace OpenGL

