#ifndef PROGRAMINPUT_HPP_JCITG7HD
#define PROGRAMINPUT_HPP_JCITG7HD
#pragma once

#include <ostream>
#include "Resource.hpp"
#include "OpenGL/Constants.hpp"


namespace OpenGL {

struct ProgramInput : public Resource {
    static constexpr GLenum interface = GL_PROGRAM_INPUT;

    GLint type = 0;
    GLint asize = 0;
    GLint location = -1;
    GLint per_patch = false;
    GLint component = 0;
    GLint referenced[MaxShaderStage] = {};

    using GLintfield = GLint(ProgramInput::*);
    static constexpr GLintfield fields[] =
            {&ProgramInput::type, &ProgramInput::asize, &ProgramInput::location,
             &ProgramInput::per_patch, &ProgramInput::component,};
    static constexpr size_t n_fields = countof(fields);

    static constexpr GLenum properties[] =
            {GL_TYPE, GL_ARRAY_SIZE, GL_LOCATION, GL_IS_PER_PATCH, GL_LOCATION_COMPONENT,
             GL_REFERENCED_BY_VERTEX_SHADER, GL_REFERENCED_BY_TESS_CONTROL_SHADER,
             GL_REFERENCED_BY_TESS_EVALUATION_SHADER, GL_REFERENCED_BY_GEOMETRY_SHADER,
             GL_REFERENCED_BY_FRAGMENT_SHADER, GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = countof(properties);
    static_assert(n_fields + MaxShaderStage == n_properties, "");

    ProgramInput(GLuint program, GLint index, GLchar* name, const GLint* values) : Resource(index,
                                                                                            name)
    {
        for (size_t i = 0; i < n_fields; ++i) {
            this->*fields[i] = values[i];
        }
        for (size_t i = 0; i < MaxShaderStage; ++i) {
            referenced[i] = values[n_fields + i];
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const ProgramInput& input)
    {
        os << static_cast<const Resource&>(input) << '\n';
        if (input.per_patch) {
            os << "[per_patch] ";
        }
        os << "type=" << nameOfDataType(input.type) << ", array_size=" << input.asize << '\n';
        os << "location=" << input.location << ", component location=" << input.component << '\n';
        os << Resource::referenced_stages(input.referenced);
        return os;
    }

};

} // namespace OpenGL

#endif /* end of include guard: PROGRAMINPUT_HPP_JCITG7HD */
