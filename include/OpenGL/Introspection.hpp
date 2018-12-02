/**
 * @file Introspection.hpp
 * @brief OpenGL Program introspection related classes and functions.
 * @sa https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_qeury
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef INTROSPECTION_HPP_JKHGRKZY
#define INTROSPECTION_HPP_JKHGRKZY

#include "Log.hpp"
#include "OpenGL/Object/Program.hpp"

#include "./Introspection/ProgramInput.hpp"
#include "./Introspection/ProgramOutput.hpp"
#include "./Introspection/Uniform.hpp"
#include "./Introspection/UniformBlock.hpp"
#include "./Introspection/SubroutineUniform.hpp"
#include "Constants.hpp"


namespace OpenGL {

// const char* InterfaceString(GLenum interface);

/// Base class of ProgramInterface<R>.
struct ProgramInterfaceBase {
    GLint max_name_length = 0;
    GLint max_n_variables = 0;
    GLint max_n_compatible_subroutines = 0;
};

/**
 * @brief Template class of different program interfaces.
 * @tparam Resource Resource type of this interface.
 */
template <typename Resource>
struct ProgramInterface : public ProgramInterfaceBase {
    static constexpr GLenum interface = Resource::interface;
    static constexpr bool InterfaceResourceNamed =
            !(interface == GL_ATOMIC_COUNTER_BUFFER ||
              interface == GL_TRANSFORM_FEEDBACK_BUFFER);
    static constexpr bool InterfaceMultipleVariables =
            (interface == GL_UNIFORM_BLOCK ||
             interface == GL_SHADER_STORAGE_BLOCK ||
             interface == GL_ATOMIC_COUNTER_BUFFER ||
             interface == GL_TRANSFORM_FEEDBACK_BUFFER);
    static constexpr bool InterfaceSubroutine =
            (interface == GL_VERTEX_SUBROUTINE_UNIFORM ||
             interface == GL_TESS_CONTROL_SUBROUTINE_UNIFORM ||
             interface == GL_TESS_EVALUATION_SUBROUTINE_UNIFORM ||
             interface == GL_GEOMETRY_SUBROUTINE_UNIFORM ||
             interface == GL_FRAGMENT_SUBROUTINE_UNIFORM ||
             interface == GL_COMPUTE_SUBROUTINE_UNIFORM);

    std::vector<Resource> resources;

    explicit ProgramInterface(const Program& program) : m_name(program.name())
    {
        // per interface properties
        GLint n_resources;
        GLuint name = program.name();
        glGetProgramInterfaceiv(name,
                                interface,
                                GL_ACTIVE_RESOURCES,
                                &n_resources);
        if (InterfaceResourceNamed) {
            glGetProgramInterfaceiv(name,
                                    interface,
                                    GL_MAX_NAME_LENGTH,
                                    &max_name_length);
        }
        if (InterfaceMultipleVariables) {
            glGetProgramInterfaceiv(name,
                                    interface,
                                    GL_MAX_NUM_ACTIVE_VARIABLES,
                                    &max_n_variables);
        }
        if (InterfaceSubroutine) {
            glGetProgramInterfaceiv(name,
                                    interface,
                                    GL_MAX_NUM_COMPATIBLE_SUBROUTINES,
                                    &max_n_compatible_subroutines);
        }
        resources.reserve(n_resources);
        // properties of resources in this interface
        const GLint n_props = Resource::n_properties;
        const GLenum* props = Resource::properties;
        GLint values[Resource::n_properties];
        auto&& name_buffer = std::make_unique<GLchar[]>(max_name_length);
        for (GLint i = 0; i < n_resources; ++i) {
            glGetProgramResourceiv(name,
                                   interface,
                                   i,
                                   n_props,
                                   props,
                                   n_props,
                                   nullptr,
                                   values);
            if (InterfaceResourceNamed) {
                glGetProgramResourceName(name,
                                         interface,
                                         i,
                                         max_name_length,
                                         nullptr,
                                         name_buffer.get());
                resources.emplace_back(name, i, name_buffer.get(), values);
            } else {
                resources.emplace_back(name, i, nullptr, values);
            }
        }
    }

    const Resource* find(const char* name) const
    {
        std::string str(name);
        for (auto& r : resources) {
            if (r.name == str) {
                return &r;
            }
        }
        fprintf(stderr,
                "in %s, %s not found\n",
                type_name<decltype(*this)>(),
                name);
        return nullptr;
    }

    void dump() const
    {
        GLsizei name_length;
        glGetObjectLabel(GL_PROGRAM, m_name, 0, &name_length, nullptr);
        auto&& name = std::make_unique<GLchar[]>(name_length + 1);
        glGetObjectLabel(GL_PROGRAM,
                         m_name,
                         name_length + 1,
                         nullptr,
                         name.get());
        Log::d("##################################################\n"
               "Dumping interface {} of program[{}]'{}'\n"
               "##################################################\n",
               type_name<Resource>(),
               m_name,
               name_length ? name.get() : "");
        for (auto&& r : resources) {
            r.dump();
        }
    }

  private:
    GLuint m_name; // should not be used other than dump()
};

using UniformInterface = ProgramInterface<Uniform>;
using UniformBlockInterface = ProgramInterface<UniformBlock>;
using ProgramInputInterface = ProgramInterface<ProgramInput>;
using ProgramOutputInterface = ProgramInterface<ProgramOutput>;
/// @TODO

} // namespace OpenGL

#endif /* end of include guard: INTROSPECTION_HPP_JKHGRKZY */
