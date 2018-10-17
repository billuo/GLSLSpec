/**
 * @file Introspection.hpp
 * @brief OpenGL Program introspection related classes and functions.
 * @sa https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_qeury
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef INTROSPECTION_HPP_JKHGRKZY
#define INTROSPECTION_HPP_JKHGRKZY

#include "OpenGL/Program.hpp"
#include <algorithm>

namespace OpenGL {

enum ShaderStage {
    VertexShader,
    TessellationControlShader,
    TessellationEvaluationShader,
    GeometryShader,
    FragmentShader,
    ComputeShader,
    MaxShaderStage,
};

struct Resource {
    GLint index = 0;
    std::string name;

    Resource(GLint index) : index(index) {}
    Resource(GLint index, GLchar* name) : index(index), name(name) {}
};

/// Base class of ProgramInterface<R>.
struct ProgramInterfaceBase {
    GLint max_name_length;
    GLint max_n_variables;
    GLint max_n_compatible_subroutines;
};
/**
 * @brief Template class of different program interfaces.
 * @tparam Resource Resource type of this interface.
 */
template <typename Resource>
struct ProgramInterface : public ProgramInterfaceBase {
    static constexpr GLenum interface = Resource::interface;
    static constexpr bool InterfaceResourceNamed =
            interface == GL_ATOMIC_COUNTER_BUFFER || interface == GL_TRANSFORM_FEEDBACK_BUFFER;

    std::vector<Resource> resources;

    ProgramInterface(const Program& program) {
        GLint n_resources;
        GLuint name = program.name();
        glGetProgramInterfaceiv(name, interface, GL_ACTIVE_RESOURCES, &n_resources);
        glGetProgramInterfaceiv(name, interface, GL_MAX_NAME_LENGTH, &max_name_length);
        glGetProgramInterfaceiv(name, interface, GL_MAX_NUM_ACTIVE_VARIABLES, &max_n_variables);
        glGetProgramInterfaceiv(name, interface, GL_MAX_NUM_COMPATIBLE_SUBROUTINES, &max_n_compatible_subroutines);
        resources.reserve(n_resources);
        //
        const GLint n_props = Resource::n_properties;
        const GLenum* props = Resource::properties;
        GLint values[Resource::n_properties];
        auto&& name_buffer = std::make_unique<GLchar[]>(max_name_length);
        for (GLint i = 0; i < n_resources; ++i) {
            glGetProgramResourceiv(name, interface, i, n_props, props, n_props, nullptr, values);
            if (InterfaceResourceNamed) {
                glGetProgramResourceName(name, interface, i, max_name_length, nullptr, name_buffer.get());
                resources.emplace_back(i, name_buffer.get(), &values[0]);
            } else {
                resources.emplace_back(i, nullptr, &values[0]);
            }
        }
    }

    const Resource* find(const char* str) const {
        std::string name(str);
        for (auto&& r : resources) {
            if (r.name == name) {
                return &r;
            }
        }
        return nullptr;
    }
};

const char* InterfaceString(GLenum interface);

/**
 * @addtogroup opengl_program_interface OpenGL Program Interface
 * @brief Interface types for uniform OpenGL program introspection.
 * @{
 */

/// Uniform variable resource
struct Uniform : public Resource {
    GLint type = 0;
    GLint asize = 0; // array size
    GLint offset = -1;
    GLint block_index = -1;
    GLint astride = -1;
    GLint mstride = -1;
    GLint row_major = 0;
    GLint atomic_index = -1;
    GLint location = -1;
    GLint referenced[MaxShaderStage] = {};

    static constexpr GLenum properties[] = {
        GL_TYPE,
        GL_ARRAY_SIZE,
        GL_OFFSET,
        GL_BLOCK_INDEX,
        GL_ARRAY_STRIDE,
        GL_MATRIX_STRIDE,
        GL_IS_ROW_MAJOR,
        GL_ATOMIC_COUNTER_BUFFER_INDEX,
        GL_LOCATION,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
    }; // XXX properties and fields must match
    static constexpr GLint(Uniform::*fields[]) = {
        &Uniform::type,    &Uniform::asize,     &Uniform::offset,       &Uniform::block_index, &Uniform::astride,
        &Uniform::mstride, &Uniform::row_major, &Uniform::atomic_index, &Uniform::location,
    };
    static constexpr size_t n_properties = countof(properties);
    static constexpr size_t n_fields = countof(fields);
    static constexpr GLenum interface = GL_UNIFORM;

    Uniform(GLint index, GLchar* name, GLint* values) : Resource(index, name) {
        for (size_t i = 0; i < n_fields; ++i) {
            this->*fields[i] = values[i];
        }
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[n_fields + i];
        }
    }
};

struct UniformBlock : public Resource {
    static constexpr GLenum interface = GL_UNIFORM_BLOCK;

    GLint binding = -1;
    GLint size = 0;
    GLint referenced[MaxShaderStage] = {};
    std::vector<Uniform> uniforms;

    UniformBlock(GLuint program, GLint index, GLchar* name, GLint* values) : Resource(index, name) {
        binding = values[0];
        size = values[1];
        GLint n_uniforms = values[2];
        uniforms.reserve(n_uniforms);
        auto&& indices = std::make_unique<GLint[]>(n_uniforms);
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[3 + i];
        }
        GLenum prop = GL_ACTIVE_VARIABLES;
        glGetProgramResourceiv(program, interface, index, 1, &prop, 1, nullptr, indices.get());
        for (GLint i = 0; i < n_uniforms; ++i) {
        }
    }

    static constexpr GLenum properties[] = {
        GL_BUFFER_BINDING,
        GL_BUFFER_DATA_SIZE,
        GL_NUM_ACTIVE_VARIABLES,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
    };
    static constexpr size_t n_properties = countof(properties);
};

/** @} */

} // namespace OpenGL

#endif /* end of include guard: INTROSPECTION_HPP_JKHGRKZY */
