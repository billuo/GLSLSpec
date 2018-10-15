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
 */
class Program : public Object<Program> {
public:
    struct Resource {
        GLint type = 0;
        GLint index = -1;
        GLint offset = 0;
        std::unique_ptr<GLchar[]> name;
        Resource() = default;
        Resource(Resource&& obj) = default;
        bool operator<(const Resource& rhs) const { return index < rhs.index; }
    };
    struct UniformBlock {
        GLuint index = -1;
        GLint size = 0;
        std::unique_ptr<GLchar[]> name;
        std::vector<Resource> uniforms;

        void Update(const std::string& uniform_name);
    };

public:
    Program() = default;
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

    /// Link all attached shaders together, forming a valid program.
    /// @note Program must be created first.
    void Link();

    /// Make shader program current in current OpenGL context.
    void Use();

    /// Query a property of an interface in a program.
    GLint GetInterface(GLenum program_interface, GLenum pname) const;

    /// Display a list of ...? @TODO
    void List(GLenum interface) const;

    /// @TODO
    const UniformBlock* GetUniformBlock(const GLchar* block_name) const;

private:
    /// Query about a property of this program.
    GLint aux_Get(GLenum param) const;
    /// Query about a property an interface of this program.
    GLint aux_GetStage(GLenum stage, GLenum pname) const;
    /// Retrive information log safely.
    std::unique_ptr<GLchar[]> aux_GetInfoLog() const;

private:
    mutable std::vector<UniformBlock> m_uniform_blocks;
    std::vector<const Shader*> m_attached_shaders;
};

} // namespace OpenGL

#endif /* end of include guard: PROGRAM_HPP_FX85CLNB */
