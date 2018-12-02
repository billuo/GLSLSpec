#ifndef UNIFORMBLOCK_HPP_NJWVEP58
#define UNIFORMBLOCK_HPP_NJWVEP58
#pragma once

#include "Uniform.hpp"
#include <vector>


namespace OpenGL {

struct UniformBlock : public Resource {
    static constexpr GLenum interface = GL_UNIFORM_BLOCK;

    GLint binding = -1;
    GLint size = 0;
    GLint referenced[MaxShaderStage] = {};
    std::vector<Uniform> uniforms;

    static constexpr GLenum properties[] =
            {GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE, GL_NUM_ACTIVE_VARIABLES,
             GL_REFERENCED_BY_VERTEX_SHADER, GL_REFERENCED_BY_TESS_CONTROL_SHADER,
             GL_REFERENCED_BY_TESS_EVALUATION_SHADER, GL_REFERENCED_BY_GEOMETRY_SHADER,
             GL_REFERENCED_BY_FRAGMENT_SHADER, GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = countof(properties);

    UniformBlock(GLuint program, GLint index, GLchar* name, const GLint* values);

    const Uniform* find(const char* name) const;

    friend std::ostream& operator<<(std::ostream& os, const UniformBlock& block);

};

} // namespace OpenGL

#endif /* end of include guard: UNIFORMBLOCK_HPP_NJWVEP58 */
