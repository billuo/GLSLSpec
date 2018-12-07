/**
 * @file Introspection.hpp
 * @brief OpenGL Program object introspection.
 * @sa https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_qeury
 * @author Zhen Luo 461652354@qq.com
 */

#include "../Constants.hpp"
#include "../Object/Program.hpp"
#include "ProgramInput.hpp"
#include "ProgramOutput.hpp"
#include "Uniform.hpp"
#include "UniformBlock.hpp"
#include "SubroutineUniform.hpp"


namespace OpenGL {

// const char* InterfaceString(GLenum interface);

namespace details {
/// Base class of ProgramInterface<R>.
struct InterfaceBase {
    GLint max_name_length = 0;
    GLint max_n_variables = 0;
    GLint max_n_compatible_subroutines = 0;
  protected:
    InterfaceBase() = default;
};
} // namespace details

/**
 * @brief Template class of different program interfaces.
 * @tparam Resource Resource type of this interface.
 */
template <typename Resource>
struct ProgramInterface : public details::InterfaceBase {
    static constexpr GLenum interface = Resource::interface;
    static constexpr bool InterfaceResourceNamed =
            !(interface == GL_ATOMIC_COUNTER_BUFFER || interface == GL_TRANSFORM_FEEDBACK_BUFFER);
    static constexpr bool InterfaceMultipleVariables = (interface == GL_UNIFORM_BLOCK ||
                                                        interface == GL_SHADER_STORAGE_BLOCK ||
                                                        interface == GL_ATOMIC_COUNTER_BUFFER ||
                                                        interface == GL_TRANSFORM_FEEDBACK_BUFFER);
    static constexpr bool InterfaceSubroutine = (interface == GL_VERTEX_SUBROUTINE_UNIFORM ||
                                                 interface == GL_TESS_CONTROL_SUBROUTINE_UNIFORM ||
                                                 interface == GL_TESS_EVALUATION_SUBROUTINE_UNIFORM ||
                                                 interface == GL_GEOMETRY_SUBROUTINE_UNIFORM ||
                                                 interface == GL_FRAGMENT_SUBROUTINE_UNIFORM ||
                                                 interface == GL_COMPUTE_SUBROUTINE_UNIFORM);

    std::vector<Resource> resources;

    explicit ProgramInterface(GLuint name)
    {
        // per interface properties
        GLint n_resources;
        glGetProgramInterfaceiv(name, interface, GL_ACTIVE_RESOURCES, &n_resources);
        if (InterfaceResourceNamed) {
            glGetProgramInterfaceiv(name, interface, GL_MAX_NAME_LENGTH, &max_name_length);
        }
        if (InterfaceMultipleVariables) {
            glGetProgramInterfaceiv(name, interface, GL_MAX_NUM_ACTIVE_VARIABLES, &max_n_variables);
        }
        if (InterfaceSubroutine) {
            glGetProgramInterfaceiv(name, interface, GL_MAX_NUM_COMPATIBLE_SUBROUTINES, &max_n_compatible_subroutines);
        }
        resources.reserve(n_resources);
        // properties of resources in this interface
        const GLint n_props = Resource::n_properties;
        const GLenum* props = Resource::properties;
        GLint values[Resource::n_properties];
        auto&& name_buffer = std::make_unique<GLchar[]>(max_name_length);
        for (GLint i = 0; i < n_resources; ++i) {
            glGetProgramResourceiv(name, interface, i, n_props, props, n_props, nullptr, values);
            if (InterfaceResourceNamed) {
                glGetProgramResourceName(name, interface, i, max_name_length, nullptr, name_buffer.get());
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
        fprintf(stderr, "in %s, %s not found\n", type_name<decltype(*this)>(), name);
        return nullptr;
    }

    template <typename ...Args>
    void assign(const char* name, Args... args) const
    {
        auto R = find(name);
        if (!R) {
            Log::e("{} not found in interface {}", name, nameOfProgramInterface(interface));
        } else {
            R->assign(args...);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ProgramInterface& interface)
    {
        os << "INTERFACE:";
        for (auto&& r : interface.resources) {
            os << '\n' << r;
        }
        return os;
    }
};

using UniformInterface = ProgramInterface<Uniform>;
using UniformBlockInterface = ProgramInterface<UniformBlock>;
using ProgramInputInterface = ProgramInterface<ProgramInput>;
using ProgramOutputInterface = ProgramInterface<ProgramOutput>;
using VertexSubroutineUniformInterface = ProgramInterface<VertexSubroutineUniform>;
using TessControlSubroutineUniformInterface = ProgramInterface<TessControlSubroutineUniform>;
using TessEvaluationSubroutineUniformInterface = ProgramInterface<TessEvaluationSubroutineUniform>;
using GeometrySubroutineUniformInterface = ProgramInterface<GeometrySubroutineUniform>;
using FragmentSubroutineUniformInterface = ProgramInterface<FragmentSubroutineUniform>;
using ComputeSubroutineUniformInterface = ProgramInterface<ComputeSubroutineUniform>;

} // namespace OpenGL

