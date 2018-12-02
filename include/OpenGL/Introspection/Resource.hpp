#ifndef RESOURCE_HPP_A4D60H9V
#define RESOURCE_HPP_A4D60H9V
#pragma once

#include "../Common.hpp"
#include "Utility.hpp"


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

    friend std::ostream& operator<<(std::ostream& os, const Resource& resource);

  protected:
    static std::string referenced_stages(const GLint referenced[MaxShaderStage]);
};

} // namespace OpenGL

#endif /* end of include guard: RESOURCE_HPP_A4D60H9V */
