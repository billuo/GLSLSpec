/**
 * @File Constants.hpp
 * @basic Provide the whole bunch of GLenums with better usability.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once
#include <Utility/Expected.hpp>
#include <OpenGL/Common.hpp>

#include <string>


namespace OpenGL {

enum ShaderStage : GLenum {
    VertexShader,
    TessellationControlShader,
    TessellationEvaluationShader,
    GeometryShader,
    FragmentShader,
    ComputeShader,
    MaxShaderStage,
};

GLsizei
sizeOfDataType(GLenum type);

GLsizei
numelOfDataType(GLenum type);

const char*
nameOfDataType(GLenum type);

expected<GLenum, std::string>
shaderTypeOfSuffix(std::string suffix);

GLenum
shaderBitOfShaderType(GLenum type);

size_t
orderOfShaderBit(GLenum bit);

const char*
nameOfShaderType(GLenum type);

const char*
nameOfProgramInterface(GLenum interface);

} // namespace OpenGL

