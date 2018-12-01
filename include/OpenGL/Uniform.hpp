#ifndef UNIFORM_HPP_VDQHP3QT
#define UNIFORM_HPP_VDQHP3QT
#pragma once

#include "OpenGL/Resource.hpp"


namespace OpenGL {

struct Uniform : public Resource {
    static constexpr GLenum interface = GL_UNIFORM;

    GLint type = 0;
    GLint asize = 0;
    GLint offset = -1;
    GLint block_index = -1;
    GLint astride = -1;
    GLint mstride = -1;
    GLint row_major = false;
    GLint atomic_index = -1;
    GLint location = -1;
    GLint referenced[MaxShaderStage] = {};

    using GLintfield = GLint(Uniform::*);
    static constexpr GLintfield fields[] =
            {&Uniform::type, &Uniform::asize, &Uniform::offset,
             &Uniform::block_index,
             &Uniform::astride, &Uniform::mstride, &Uniform::row_major,
             &Uniform::atomic_index, &Uniform::location,};
    static constexpr size_t n_fields = countof(fields);

    static constexpr GLenum properties[] =
            {GL_TYPE, GL_ARRAY_SIZE, GL_OFFSET, GL_BLOCK_INDEX, GL_ARRAY_STRIDE,
             GL_MATRIX_STRIDE, GL_IS_ROW_MAJOR, GL_ATOMIC_COUNTER_BUFFER_INDEX,
             GL_LOCATION, GL_REFERENCED_BY_VERTEX_SHADER,
             GL_REFERENCED_BY_TESS_CONTROL_SHADER,
             GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
             GL_REFERENCED_BY_GEOMETRY_SHADER,
             GL_REFERENCED_BY_FRAGMENT_SHADER,
             GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = countof(properties);

    Uniform(GLuint program, GLint index, GLchar* name, const GLint* values)
            : Resource(index, name)
    {
        UNUSED(program);
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
        if (location == -1) {
            fprintf(stderr,
                    "type=%s, block_index=%d\n",
                    nameOfType(type),
                    block_index);
        } else {
            fprintf(stderr,
                    "type=%s, location=%d\n",
                    nameOfType(type),
                    location);
        }
        Resource::dump_referenced(referenced);
    }

  private:
    friend struct UniformBlock;

    void
    dump_from_block() const
    {
        putc('\t', stderr);
        Resource::dump();
        fprintf(stderr, "\ttype=%s, offset=%d\n", nameOfType(type), offset);
    }
};
} // namespace OpenGL

#endif /* end of include guard: UNIFORM_HPP_VDQHP3QT */
