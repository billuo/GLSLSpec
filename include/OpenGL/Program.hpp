/**
 * @file Program.hpp
 * @brief Encapsulates OpenGL shader program objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef PROGRAM_HPP_FX85CLNB
#define PROGRAM_HPP_FX85CLNB
#pragma once

#include "OpenGL/Buffer.hpp"
#include "OpenGL/Shader.hpp"
#include <vector>


namespace OpenGL {

struct UniformBlock;

/**
 * @class Program
 * @brief Encapsulates OpenGL shader program objects.
 */
class Program : public Object {

    // non-standard object name 'pool'
    static struct {
        Name
        Get()
        {
            GLuint name = glCreateProgram();
            return Name(name);
        }

        void
        Put(Name&& name)
        { glDeleteProgram(name.get()); }
    } Pool;

  public:
    static void
    Use(const Program& prog)
    { glUseProgram(static_cast<GLuint>(prog.m_name)); }

  public:
    explicit Program(const GLchar* label = nullptr) : Object(Pool.Get(),
                                                             label,
                                                             GL_PROGRAM)
    {}

    Program(Program&&) = default;

    Program&
    operator=(Program&&) = default;

    ~Program()
    { Pool.Put(std::move(m_name)); }

    /// Attach a shader to this program.
    Program&
    attach(const Shader* shader);

    /// Attach shaders to this program.
    Program&
    attach(const std::vector<const Shader*>& shaders);

    /// Link all attached shaders together, forming a valid program.
    /// @note Program must be created first.
    Program&
    link();

  private:
    /// Query about a property of this program.
    GLint
    aux_get(GLenum param) const;

    /// Query about a property an interface of this program.
    GLint
    aux_getStage(GLenum stage, GLenum pname) const;

    /// Retrive information log safely.
    std::unique_ptr<GLchar[]>
    aux_getInfoLog() const;

  private:
    std::vector<const Shader*> m_attached_shaders;
};
} // namespace OpenGL

#endif /* end of include guard: PROGRAM_HPP_FX85CLNB */
