/**
 * @File ProgramInput.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <ostream>
#include <OpenGL/Introspection/Resource.hpp>
#include <OpenGL/Constants.hpp>


namespace OpenGL {

struct ProgramInput : public Resource {
    static constexpr GLenum interface = GL_PROGRAM_INPUT;

    GLint type{0};
    GLint asize{0};
    GLint location{-1};
    GLint per_patch{false};
    GLint component{0};
    GLint referenced[MaxShaderStage]{};

    using GLintfield = GLint(ProgramInput::*);
    static constexpr GLintfield fields[]
            {&ProgramInput::type, &ProgramInput::asize, &ProgramInput::location,
             &ProgramInput::per_patch,};
    static constexpr size_t n_fields{numel(fields)};

    static constexpr GLenum properties[]
            {GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION, GL_IS_PER_PATCH,
             GL_REFERENCED_BY_VERTEX_SHADER, GL_REFERENCED_BY_TESS_CONTROL_SHADER,
             GL_REFERENCED_BY_TESS_EVALUATION_SHADER, GL_REFERENCED_BY_GEOMETRY_SHADER,
             GL_REFERENCED_BY_FRAGMENT_SHADER, GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties{numel(properties)};
    static_assert(n_fields + MaxShaderStage == n_properties);

    ProgramInput(GLuint program, GLint index, GLchar* name, const GLint* values);

    friend std::ostream& operator<<(std::ostream& os, const ProgramInput& input);

};

} // namespace OpenGL

