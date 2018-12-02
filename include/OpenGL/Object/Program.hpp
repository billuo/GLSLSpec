/**
 * @file Program.hpp
 * @brief Encapsulates OpenGL shader program objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef PROGRAM_HPP_FX85CLNB
#define PROGRAM_HPP_FX85CLNB
#pragma once

#include "Shader.hpp"
#include <set>


namespace OpenGL {

/// OpenGL shader program object
class Program : public Object {
  public:
    static void Use(const Program& prog)
    { glUseProgram(static_cast<GLuint>(prog.m_name)); }

    explicit Program(const GLchar* label = nullptr) : Object(pool.Get(),
                                                             label,
                                                             GL_PROGRAM)
    {}

    Program(Program&&) = default;
    Program& operator=(Program&&) = default;

    ~Program()
    { pool.Put(std::move(m_name)); }

    /// Attach a shader to this program.
    Program& attach(shared_shader shader);
    /// Attach shaders to this program.
    Program& attach(std::initializer_list<shared_shader> shaders);
    /// Link all attached shaders together, forming a valid program.
    Program& link();

  private:
    // specialized object name pool
    static struct {
        Name Get()
        {
            GLuint name = glCreateProgram();
            return Name(name);
        }

        void Put(Name&& name)
        { glDeleteProgram(name.get()); }
    } pool;

    /// Query about a property of this program.
    GLint aux_get(GLenum param) const;
    /// Query about a property an interface of this program.
    GLint aux_getStage(GLenum stage, GLenum pname) const;
    /// Retrive information log safely.
    std::unique_ptr<GLchar[]> aux_getInfoLog() const;

    std::set<shared_shader> m_attached_shaders;
};

} // namespace OpenGL

#endif /* end of include guard: PROGRAM_HPP_FX85CLNB */
