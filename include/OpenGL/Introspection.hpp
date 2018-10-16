/**
 * @file Introspection.hpp
 * @brief OpenGL Program introspection related classes and functions.
 * @sa https://www.khronos.org/opengl/wiki/Program_Introspection#Interface_qeury
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef INTROSPECTION_HPP_JKHGRKZY
#define INTROSPECTION_HPP_JKHGRKZY

#include "OpenGL/Program.hpp"

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

    // GL_ATOMIC_COUNTER_BUFFER and GL_TRANSFORM_FEEDBACK_BUFFER resources are not assigned names.
    Resource(GLint index) : index(index) {}
    Resource(GLint index, GLchar* name) : index(index), name(name) {}
};

/**
 * @brief Template class of different program interfaces.
 * @tparam Resource Resource type of this interface.
 */
template <typename Resource>
struct ProgramInterface {
    GLint max_name_length;
    GLint max_n_variables;
    GLint max_n_compatible_subroutines;
    std::vector<std::unique_ptr<Resource>> resources;

    ProgramInterface(const Program& program) {
        GLenum interface = Resource::interface;
        GLint n_resources;
        GLuint name = program.name();
        glGetProgramInterfaceiv(name, interface, GL_ACTIVE_RESOURCES, &n_resources);
        glGetProgramInterfaceiv(name, interface, GL_MAX_NAME_LENGTH, &max_name_length);
        glGetProgramInterfaceiv(name, interface, GL_MAX_NUM_ACTIVE_VARIABLES, &max_n_variables);
        glGetProgramInterfaceiv(name, interface, GL_MAX_NUM_COMPATIBLE_SUBROUTINES, &max_n_compatible_subroutines);
        resources.reserve(n_resources);
        GLint values[Resource::n_properties];
        GLchar buffer[max_name_length];
        for (GLint i = 0; i < n_resources; ++i) {
            if (interface != GL_ATOMIC_COUNTER_BUFFER && interface != GL_TRANSFORM_FEEDBACK_BUFFER) {
                glGetProgramResourceName(program.name(), interface, i, max_name_length, nullptr, buffer);
                glGetProgramResourceiv(program.name(), interface, i, Resource::n_properties, Resource::properties,
                                       Resource::n_properties, nullptr, values);
                resources.emplace_back(new Resource(i, name, values));
            } else {
                glGetProgramResourceiv(program.name(), interface, i, Resource::n_properties, Resource::properties,
                                       Resource::n_properties, nullptr, values);
                resources.emplace_back(new Resource(i, values));
            }
        }
    }
};

const char* InterfaceString(GLenum interface);

/**
 * @addtogroup opengl_program_interface OpenGL Program Interface
 * @brief Interface types for uniform OpenGL program introspection.
 * @{
 */

struct Uniform : public ProgramInterface {
    using Base = ProgramInterface;

    static constexpr GLenum interface = GL_UNIFORM;
    struct Resource : public ::OpenGL::Resource {
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

        virtual void init_values(GLint* values) override {
            for (size_t i = 0; i < n_fields; ++i) {
                this->*fields[i] = values[i];
            }
            for (size_t i = 0; i < MaxShaderStage; ++i) {
                referenced[i] = values[n_fields + i];
            }
        }

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
        static constexpr GLint(Resource::*fields[]) = {
            &Resource::type,        &Resource::asize,        &Resource::offset,
            &Resource::block_index, &Resource::astride,      &Resource::mstride,
            &Resource::row_major,   &Resource::atomic_index, &Resource::location,
        };
        static constexpr size_t n_properties = countof(properties);
        static constexpr size_t n_fields = countof(fields);
    };

public:
    std::vector<Resource> resources;

    Uniform(const Program& program) : ProgramInterface(program, interface) {}
};

struct UniformBlock : public ProgramInterface {
private:
    using Base = ProgramInterface;

public:
    static constexpr GLenum interface = GL_UNIFORM_BLOCK;

    struct Resource {
        std::unique_ptr<GLint[]> variables;
        GLint binding = -1;
        GLint size = 0;
        GLint n_variables = 1;
        GLint referenced[MaxShaderStage] = {};

        void init(GLint index, GLchar* name, GLint* values) {}

        static constexpr GLenum properties[] = {
            GL_BUFFER_BINDING,
            GL_BUFFER_DATA_SIZE,
            GL_NUM_ACTIVE_VARIABLES,
            GL_ACTIVE_VARIABLES,
            GL_REFERENCED_BY_VERTEX_SHADER,
            GL_REFERENCED_BY_TESS_CONTROL_SHADER,
            GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
            GL_REFERENCED_BY_GEOMETRY_SHADER,
            GL_REFERENCED_BY_FRAGMENT_SHADER,
            GL_REFERENCED_BY_COMPUTE_SHADER,
        };
        static constexpr size_t n_properties = countof(properties);
    };

public:
    std::vector<Resource> resources;

    UniformBlock(const Program& program) : Base(program, interface) {
        resources.resize(Base::n_resources);
        GLint values[Resource::n_properties];
        GLchar name[Base::max_name_length];
        for (GLint i = 0; i < Base::n_resources; ++i) {
            glGetProgramResourceName(program.name(), interface, i, Base::max_name_length, nullptr, name);
            glGetProgramResourceiv(program.name(), interface, i, Resource::n_properties, Resource::properties,
                                   Resource::n_properties, nullptr, values);
            resources[i].init(i, name, values);
        }
    }
};

/** @} */

} // namespace OpenGL

#endif /* end of include guard: INTROSPECTION_HPP_JKHGRKZY */
