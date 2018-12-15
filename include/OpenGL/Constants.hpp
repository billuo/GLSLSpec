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

///@note Can be easily used as indices to array.
enum class ShaderStage : GLenum {
    Vertex,
    TessellationControl,
    TessellationEvaluation,
    Geometry,
    Fragment,
    Compute,
    Max,
};

static constexpr size_t MaxShaderStage = underlying_cast(ShaderStage::Max);

//region Shader data types

GLsizei
sizeOfDataType(GLenum type);

GLsizei
numelOfDataType(GLenum type);

GLenum
componentTypeOfDataType(GLenum type);

std::string
nameOfDataType(GLenum type);

//endregion

//region Shader Enums

/// string -> GLenum
expected<GLenum, std::string>
shaderTypeOfSuffix(std::string suffix);

/// GLenum -> string
std::string
nameOfShaderType(GLenum type);

/// GLenum -> GLbitfield
GLbitfield
bitOfShaderType(GLenum type);

/// GLenum -> ShaderStage
ShaderStage
stageOfShaderType(GLenum type);

/// GLbitfield -> enum class ShaderStage
ShaderStage
stageOfShaderBit(GLbitfield bit);

/// enum class ShaderStage -> GLbitfield
GLbitfield
bitOfShaderStage(ShaderStage stage);


//endregion

const char*
nameOfProgramInterface(GLenum interface);

} // namespace OpenGL

