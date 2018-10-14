/**
 * @file Program.hpp
 * @brief Encapsulates OpenGL shader program objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef PROGRAM_HPP_FX85CLNB
#define PROGRAM_HPP_FX85CLNB
#pragma once

#include "OpenGL/Shader.hpp"
#include <vector>

namespace OpenGL {

/**
 * @class Program
 * @brief Encapsulates OpenGL shader program objects.
 * @details A shader program groups together several shader objects, linking (possibly after compiling) them before put
 * into use.
 */
class Program : public Object<Program> {
public:
    struct Resource {
        GLint type;
        GLint index;
        GLint offset;
        std::string name;
        Resource() : type(0), index(-1), offset(0), name() {}
        bool operator<(const Resource& rhs) const { return index < rhs.index; }
    };
    struct UniformBlock {
        GLuint index;
        GLint size;
        std::string name;
        std::vector<Resource> uniforms;

        void Update(const std::string& uniform_name);
    };

public:
    Program() : Object<Program>(), m_uniform_blocks() {}
    ~Program() {
        if (Initialized()) {
            Delete();
        }
    }

    void Create() {
        if (aux_CheckInitialized(false)) {
            m_name = glCreateProgram();
        }
    }

    void Delete() {
        if (aux_CheckInitialized(true)) {
            glDeleteProgram(m_name);
        }
    }

    /// Attach shader to this program.
    void Attach(const Shader& shader);

    /// Detach shader from this program.
    void Detach(const Shader& shader);

    /// Link this program.
    void Link();

    /// Make shader program current in current OpenGL context.
    void Use();

    /// Query a property of an interface in a program.
    GLint GetInterface(GLenum program_interface, GLenum pname) const;

    /// Display a list of ...? @TODO
    void List(GLenum interface) const;

    /// @TODO
    const UniformBlock* GetUniformBlock(const GLchar* name) const;

    /// Retrive information log safely.
    std::string GetInfoLog() const;

private:
    Program(const Program&);
    Program& operator=(const Program&);

    GLint aux_Get(GLenum param) const;
    GLint aux_GetStage(GLenum stage, GLenum pname) const;

private:
    mutable std::vector<UniformBlock> m_uniform_blocks;
};

} // namespace OpenGL

#endif /* end of include guard: PROGRAM_HPP_FX85CLNB */
