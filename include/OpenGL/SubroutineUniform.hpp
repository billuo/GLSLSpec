#ifndef SUBROUTINEUNIFORM_HPP_HFZJLTZH
#define SUBROUTINEUNIFORM_HPP_HFZJLTZH
#pragma once

#include "OpenGL/Resource.hpp"
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

    SubroutineUniform(GLuint program, GLenum stage, GLint index, GLchar* name, const GLint* values) : Resource(index, name)
    {
        asize = values[0];
        location = values[1];
        GLint n_subroutines;
        glGetProgramStageiv(program, stage, GL_ACTIVE_SUBROUTINES, &n_subroutines);
        for (GLint i = 0; i < n_subroutines; ++i) {
            GLint name_length;
            glGetActiveSubroutineName(program, stage, i, 0, &name_length, nullptr);
            auto&& name = std::make_unique<GLchar[]>(name_length);
            glGetActiveSubroutineName(program, stage, i, name_length, nullptr, name.get());
            subroutines.emplace_back(i, name.get());
        }
    }

    void dump() const
    {
        Resource::dump();
        fprintf(stderr, "array_size=%d, location=%d\n", asize, location);
        for (auto& sr : subroutines) {
            sr.dump();
        }
    }
};

struct VertexSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_VERTEX_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_VERTEX_SHADER;

    VertexSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values) : SubroutineUniform(program,
                                                                                                          stage,
                                                                                                          index,
                                                                                                          name,
                                                                                                          values)
    {}
};

struct TessControlSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_TESS_CONTROL_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_TESS_CONTROL_SHADER;

    TessControlSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values) : SubroutineUniform(program,
                                                                                                               stage,
                                                                                                               index,
                                                                                                               name,
                                                                                                               values)
    {}
};

struct TessEvaluationSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_TESS_EVALUATION_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_TESS_EVALUATION_SHADER;

    TessEvaluationSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values) : SubroutineUniform(
            program,
            stage,
            index,
            name,
            values)
    {}
};

struct GeometrySubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_GEOMETRY_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_GEOMETRY_SHADER;

    GeometrySubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values) : SubroutineUniform(program,
                                                                                                            stage,
                                                                                                            index,
                                                                                                            name,
                                                                                                            values)
    {}
};

struct FragmentSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_FRAGMENT_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_FRAGMENT_SHADER;

    FragmentSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values) : SubroutineUniform(program,
                                                                                                            stage,
                                                                                                            index,
                                                                                                            name,
                                                                                                            values)
    {}
};

struct ComputeSubroutineUniform : public SubroutineUniform {
    static constexpr GLenum interface = GL_COMPUTE_SUBROUTINE_UNIFORM;
    static constexpr GLenum stage = GL_COMPUTE_SHADER;

    ComputeSubroutineUniform(GLuint program, GLint index, GLchar* name, GLint* values) : SubroutineUniform(program,
                                                                                                           stage,
                                                                                                           index,
                                                                                                           name,
                                                                                                           values)
    {}
};
} // namespace OpenGL

#endif /* end of include guard: SUBROUTINEUNIFORM_HPP_HFZJLTZH */
