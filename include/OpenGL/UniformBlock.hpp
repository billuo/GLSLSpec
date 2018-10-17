#ifndef UNIFORMBLOCK_HPP_NJWVEP58
#define UNIFORMBLOCK_HPP_NJWVEP58
#pragma once

#include "OpenGL/Uniform.hpp"
#include <vector>

namespace OpenGL {

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
        //
        // per block property
        binding = values[0];
        size = values[1];
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[3 + i];
        }
        //
        // prepare to iterate uniforms in the block
        const GLint n_uniforms = values[2];
        const GLint n_props = Uniform::n_properties;
        const GLenum* props = Uniform::properties;
        // array of uniform indices
        GLenum active_variables = GL_ACTIVE_VARIABLES;
        auto&& indices = std::make_unique<GLint[]>(n_uniforms);
        glGetProgramResourceiv(program, interface, index, 1, &active_variables, n_uniforms, nullptr, indices.get());
        // uniform name
        GLint max_name_length;
        glGetProgramInterfaceiv(program, Uniform::interface, GL_MAX_NAME_LENGTH, &max_name_length);
        auto&& name_buffer = std::make_unique<GLchar[]>(max_name_length);
        // uniform properties
        auto&& value_buffer = std::make_unique<GLint[]>(n_props);
        // iterate
        uniforms.reserve(n_uniforms);
        for (GLint i = 0; i < n_uniforms; ++i) {
            /// OPT simply fetch from interface GL_UNIFORM with known indices[] to avoid duplicate.
            glGetProgramResourceiv(program, Uniform::interface, indices[i], n_props, props, n_props, nullptr,
                                   value_buffer.get());
            glGetProgramResourceName(program, Uniform::interface, indices[i], max_name_length, nullptr,
                                     name_buffer.get());
            uniforms.emplace_back(program, indices[i], name_buffer.get(), value_buffer.get());
        }
    }

    const Uniform* find(const char* name) const {
        std::string str(name);
        for (auto& u : uniforms) {
            if (u.name == str) {
                return &u;
            }
        }
        return nullptr;
    }

    void dump() const {
        Resource::dump();
        fprintf(stderr, "binding=%d, size=%d, uniforms:\n", binding, size);
        for (auto&& u : uniforms) {
            u.dump_from_block();
        }
        Resource::dump_referenced(referenced);
    }
};

} // namespace OpenGL

#endif /* end of include guard: UNIFORMBLOCK_HPP_NJWVEP58 */
