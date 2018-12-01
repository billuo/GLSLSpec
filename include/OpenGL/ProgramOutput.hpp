#ifndef PROGRAMOUTPUT_HPP_DN7NRFC6
#define PROGRAMOUTPUT_HPP_DN7NRFC6
#pragma once

#include "OpenGL/Resource.hpp"


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
            {&ProgramOutput::type, &ProgramOutput::asize,
             &ProgramOutput::location,
             &ProgramOutput::location_index, &ProgramOutput::per_patch,
             &ProgramOutput::component,};
    static constexpr size_t n_fields = countof(fields);

    static constexpr GLenum properties[] = {GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION,
                                            GL_LOCATION_INDEX, // dual source blending
                                            GL_IS_PER_PATCH,
                                            GL_LOCATION_COMPONENT,
                                            GL_REFERENCED_BY_VERTEX_SHADER,
                                            GL_REFERENCED_BY_TESS_CONTROL_SHADER,
                                            GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
                                            GL_REFERENCED_BY_GEOMETRY_SHADER,
                                            GL_REFERENCED_BY_FRAGMENT_SHADER,
                                            GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = countof(properties);
    static_assert(n_fields + MaxShaderStage == n_properties, "");

    ProgramOutput(GLuint program, GLint index, GLchar* name,
                  const GLint* values) : Resource(index, name)
    {
        for (size_t i = 0; i < n_fields; ++i) {
            this->*fields[i] = values[i];
        }
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[n_fields + i];
        }
    }

    void
    dump() const
    {
        Resource::dump();
        fprintf(stderr,
                "type=%s, array_size=%d, location=%d, location_index=%d\nper_patch=%s, component location=%d\n",
                nameOfType(type),
                asize,
                location,
                location_index,
                per_patch ? "true" : "false",
                component);
        Resource::dump_referenced(referenced);
    }
};
} // namespace OpenGL

#endif /* end of include guard: PROGRAMOUTPUT_HPP_DN7NRFC6 */
