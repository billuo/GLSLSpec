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

    void dump() const { fprintf(stderr, "index=%d, name='%s'\n", index, name.c_str()); }
};

const char* InterfaceString(GLenum interface);

/**
 * @addtogroup opengl_program_resource OpenGL Program Resource
 * @brief Resource types for OpenGL program interfaces introspection.
 * @{
 */

struct Uniform : public Resource {
    static constexpr GLenum interface = GL_UNIFORM;

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

    using GLintfield = GLint(Uniform::*);
    static constexpr GLintfield fields[] = {
        &Uniform::type,    &Uniform::asize,     &Uniform::offset,       &Uniform::block_index, &Uniform::astride,
        &Uniform::mstride, &Uniform::row_major, &Uniform::atomic_index, &Uniform::location,
    }; // XXX properties and fields must match
    static constexpr size_t n_fields = countof(fields);

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
    };
    static constexpr size_t n_properties = countof(properties);

    Uniform(GLuint program, GLint index, GLchar* name, GLint* values) : Resource(index, name) {
        UNUSED(program);
        for (size_t i = 0; i < n_fields; ++i) {
            this->*fields[i] = values[i];
        }
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[n_fields + i];
        }
    }

    void dump() const {
        Resource::dump();
        fprintf(stderr, "type=%s, location=%d\n", TypeString(type), location);
    }

private:
    friend struct UniformBlock;
    void dump_from_block() const {
        Resource::dump();
        fprintf(stderr, "type=%s, offset=%d\n", TypeString(type), offset);
    }
};

struct UniformBlock : public Resource {
    static constexpr GLenum interface = GL_UNIFORM_BLOCK;

    GLint binding = -1;
    GLint size = 0;
    GLint referenced[MaxShaderStage] = {};
    std::vector<Uniform> uniforms;

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

    UniformBlock(GLuint program, GLint index, GLchar* name, GLint* values) : Resource(index, name) {
        binding = values[0];
        size = values[1];
        const GLint n_uniforms = values[2];
        uniforms.reserve(n_uniforms);
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[3 + i];
        }
        GLenum active_variables = GL_ACTIVE_VARIABLES;
        auto&& indices = std::make_unique<GLint[]>(n_uniforms);
        GLint max_name_length;
        glGetProgramResourceiv(program, interface, index, 1, &active_variables, n_uniforms, nullptr, indices.get());
        glGetProgramInterfaceiv(program, Uniform::interface, GL_MAX_NAME_LENGTH, &max_name_length);
        const GLint n_props = Uniform::n_properties;
        const GLenum* props = Uniform::properties;
        auto&& name_buffer = std::make_unique<GLchar[]>(max_name_length);
        auto&& value_buffer = std::make_unique<GLint[]>(n_props);
        for (GLint i = 0; i < n_uniforms; ++i) {
            glGetProgramResourceiv(program, Uniform::interface, indices[i], n_props, props, n_props, nullptr,
                                   value_buffer.get());
            glGetProgramResourceName(program, Uniform::interface, indices[i], max_name_length, nullptr,
                                     name_buffer.get());
            uniforms.emplace_back(program, indices[i], name_buffer.get(), value_buffer.get());
        }
    }
    void dump() const {
        Resource::dump();
        fprintf(stderr, "binding=%d, size=%d, uniforms:\n", binding, size);
        for (auto&& u : uniforms) {
            // fprintf(stderr, "index=%d ", u);
            u.dump_from_block();
        }
    }
};

/** @} */

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
            !(interface == GL_ATOMIC_COUNTER_BUFFER || interface == GL_TRANSFORM_FEEDBACK_BUFFER);
    static constexpr bool InterfaceMultipleVariables =
            (interface == GL_UNIFORM_BLOCK || interface == GL_SHADER_STORAGE_BLOCK ||
             interface == GL_ATOMIC_COUNTER_BUFFER || interface == GL_TRANSFORM_FEEDBACK_BUFFER);
    static constexpr bool InterfaceSubroutine =
            (interface == GL_VERTEX_SUBROUTINE_UNIFORM || interface == GL_TESS_CONTROL_SUBROUTINE_UNIFORM ||
             interface == GL_TESS_EVALUATION_SUBROUTINE_UNIFORM || interface == GL_GEOMETRY_SUBROUTINE_UNIFORM ||
             interface == GL_FRAGMENT_SUBROUTINE_UNIFORM || interface == GL_COMPUTE_SUBROUTINE_UNIFORM);

    std::vector<Resource> resources;

    ProgramInterface(const Program& program) {
        // per interface properties
        GLint n_resources;
        GLuint name = program.name();
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

    const Resource* find(const char* str) const {
        std::string name(str);
        for (auto&& r : resources) {
            if (r.name == name) {
                return &r;
            }
        }
        return nullptr;
    }

    void dump() const {
        fprintf(stderr, "Dumping interface for %s:\n", type_name<Resource>());
        for (auto&& r : resources) {
            r.dump();
        }
    }
};

} // namespace OpenGL

#endif /* end of include guard: INTROSPECTION_HPP_JKHGRKZY */
