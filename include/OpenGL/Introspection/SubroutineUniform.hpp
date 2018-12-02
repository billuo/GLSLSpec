#ifndef SUBROUTINEUNIFORM_HPP_HFZJLTZH
#define SUBROUTINEUNIFORM_HPP_HFZJLTZH
#pragma once

#include "Resource.hpp"
#include <vector>


namespace OpenGL {

/// not a category of interface per se!
struct SubroutineUniform : public Resource {
  protected:
    using Subroutine = Resource;

  public:
    GLint asize = 0;
    GLint location = -1;
    std::vector<Subroutine> subroutines;
    static constexpr GLenum properties[] = {GL_ARRAY_SIZE, GL_LOCATION,};
    static constexpr size_t n_properties = countof(properties);

    SubroutineUniform(GLuint program, GLenum stage, GLint index, GLchar* name, const GLint* values);

    friend std::ostream& operator<<(std::ostream& os, const SubroutineUniform& sub_uniform);

};

struct VertexSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_VERTEX_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_VERTEX_SHADER;

    VertexSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values)
            : SubroutineUniform(program, stage, index, name, values)
    {}
};

struct TessControlSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_TESS_CONTROL_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_TESS_CONTROL_SHADER;

    TessControlSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values)
            : SubroutineUniform(program, stage, index, name, values)
    {}
};

struct TessEvaluationSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_TESS_EVALUATION_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_TESS_EVALUATION_SHADER;

    TessEvaluationSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values)
            : SubroutineUniform(program, stage, index, name, values)
    {}
};

struct GeometrySubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_GEOMETRY_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_GEOMETRY_SHADER;

    GeometrySubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values)
            : SubroutineUniform(program, stage, index, name, values)
    {}
};

struct FragmentSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_FRAGMENT_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_FRAGMENT_SHADER;

    FragmentSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values)
            : SubroutineUniform(program, stage, index, name, values)
    {}
};

struct ComputeSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_COMPUTE_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_COMPUTE_SHADER;

    ComputeSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values)
            : SubroutineUniform(program, stage, index, name, values)
    {}
};
} // namespace OpenGL

#endif /* end of include guard: SUBROUTINEUNIFORM_HPP_HFZJLTZH */
