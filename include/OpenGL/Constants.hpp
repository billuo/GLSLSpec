/**
 * @File Constants.hpp
 * @basic Provide the whole bunch of GLenums with better usability.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once
#include <Utility/Expected.hpp>
#include <Utility/Misc.hpp>
#include <OpenGL/Common.hpp>

#include <string>


namespace OpenGL {

enum class ShaderStage : GLenum {
    Vertex,
    TessellationControl,
    TessellationEvaluation,
    Geometry,
    Fragment,
    Compute,
    Max,
};

static constexpr auto MaxShaderStage = underlying_cast(ShaderStage::Max);

GLsizei
sizeOfDataType(GLenum type);

GLsizei
numelOfDataType(GLenum type);

const char*
nameOfDataType(GLenum type);

expected<GLenum, std::string>
shaderTypeOfSuffix(std::string suffix);

GLenum
typeOfShaderBit(GLbitfield bit);

GLenum
bitOfShaderType(GLenum type);

ShaderStage
stageOfShaderBit(GLbitfield bit);

GLbitfield
bitOfShaderStage(ShaderStage stage);

const char*
nameOfShaderType(GLenum type);

const char*
nameOfProgramInterface(GLenum interface);

} // namespace OpenGL

