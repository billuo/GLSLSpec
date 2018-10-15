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
    Program() = default;
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;
    Program(Program&&) = default;
    Program& operator=(Program&&) = default;
    ~Program() {
        if (Initialized()) {
            Delete();
        }
    }

    void Create() {
        if (aux_CheckInitialized(false)) {
            SetName(glCreateProgram());
        }
    }

    void Delete() {
        if (aux_CheckInitialized(true)) {
            glDeleteProgram(Name());
        }
    }

    /// Attach a shader to this program.
    void Attach(const Shader* shader);
    /// Attach shaders to this program.
    void Attach(const std::vector<const Shader*>& shaders);

    /// Detach shader from this program.
    // void Detach(const Shader* shader);

    /// Link all attached shaders together, forming a valid program. Program must be created first.
    void Link();

    /// Make shader program current in current OpenGL context.
    void Use();

    /// Query a property of an interface in a program.
    GLint GetInterface(GLenum program_interface, GLenum pname) const;

    /// Display a list of ...? @TODO
    void List(GLenum interface) const;

    /// @TODO
    const UniformBlock* GetUniformBlock(const GLchar* name) const;

private:
    /// Query about a property of this program.
    GLint aux_Get(GLenum param) const;
    /// Query about a property an interface of this program.
    GLint aux_GetStage(GLenum stage, GLenum pname) const;
    /// Retrive information log safely.
    std::string aux_GetInfoLog() const;

private:
    mutable std::vector<UniformBlock> m_uniform_blocks;
    std::vector<const Shader*> m_attached_shaders;
};

} // namespace OpenGL

#endif /* end of include guard: PROGRAM_HPP_FX85CLNB */
