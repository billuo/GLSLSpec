#ifndef RESOURCE_HPP_A4D60H9V
#define RESOURCE_HPP_A4D60H9V
#pragma once

#include "OpenGL/Common.hpp"
#include "Utility.hpp"
#include <string>


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

    explicit Resource(GLint index) : index(index)
    {}

    Resource(GLint index, GLchar* name) : index(index), name(name)
    {}

    void
    dump() const
    { fprintf(stderr, "index=%d, name='%s'\n", index, name.c_str()); }

  protected:
    static void
    dump_referenced(const GLint referenced[MaxShaderStage]);
};
} // namespace OpenGL

#endif /* end of include guard: RESOURCE_HPP_A4D60H9V */
