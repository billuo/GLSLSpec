/**
 * @File ProgramOutput.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <ostream>
#include "Resource.hpp"
#include "../Constants.hpp"


namespace OpenGL {

struct ProgramOutput : public Resource {
    static constexpr GLenum interface = GL_PROGRAM_OUTPUT;

    GLint type = 0;
    GLint asize = 0;
    GLint location = -1;
    GLint location_index = -1;
    GLint per_patch = false;
    GLint component = 0;
    GLint referenced[MaxShaderStage] = {};

    using GLintfield = GLint(ProgramOutput::*);
    static constexpr GLintfield fields[] =
            {&ProgramOutput::type, &ProgramOutput::asize, &ProgramOutput::location,
             &ProgramOutput::location_index, &ProgramOutput::per_patch,};
    static constexpr size_t n_fields = numel(fields);

    static constexpr GLenum properties[] =
            {GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION, GL_LOCATION_INDEX, // dual source blending
             GL_IS_PER_PATCH, GL_REFERENCED_BY_VERTEX_SHADER,
             GL_REFERENCED_BY_TESS_CONTROL_SHADER, GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
             GL_REFERENCED_BY_GEOMETRY_SHADER, GL_REFERENCED_BY_FRAGMENT_SHADER,
             GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = numel(properties);
    static_assert(n_fields + MaxShaderStage == n_properties, "");

    ProgramOutput(GLuint program, GLint index, GLchar* name, const GLint* values);

    friend std::ostream& operator<<(std::ostream& os, const ProgramOutput& output);

};

} // namespace OpenGL

