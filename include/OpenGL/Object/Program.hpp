/**
 * @file Program.hpp
 * @brief Encapsulates OpenGL shader program objects.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Shader.hpp"
#include <set>


namespace OpenGL {

/// OpenGL shader program object
class Program : public Object {
    static auto& pool()
    {
        static struct {
            Name get()
            {
                GLuint name = glCreateProgram();
                return Name(name);
            }

            void put(Name&& name)
            { glDeleteProgram(name.get()); }
        } singleton;
        return singleton;
    }

  public:
    static void Use(const Program& prog)
    { glUseProgram(prog.name()); }

    explicit Program(const GLchar* label = nullptr) : Object(pool().get())
    {
        if (label) {
            Object::label(label, GL_PROGRAM);
        }
    }

    Program(Program&&) = default;
    Program& operator=(Program&&) = default;

    ~Program()
    { pool().put(std::move(m_name)); }

    Program& attach(const Shader& shader);
    /// Link all attached shaders together, forming a valid program.
    Program& link();

    /// Query about a parameter
    GLint get(GLenum param) const;
    /// Query about a property an interface
    GLint get_stage(GLenum stage, GLenum pname) const;
    /// Retrive information log safely.
    std::unique_ptr<GLchar[]> get_info_log() const;
    /// Set a parameter.
    void set(GLenum param, GLint value);
};

} // namespace OpenGL

