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
suffix_shader_type(std::string suffix);

/// GLenum -> string
std::string
shader_type_name(GLenum type);

/// GLenum -> GLbitfield
GLbitfield
shader_type_bit(GLenum type);

/// GLenum -> ShaderStage
ShaderStage
shader_type_stage(GLenum type);

/// GLbitfield -> enum class ShaderStage
/// @note @p bit must not be OR-ed.
ShaderStage
shader_bit_stage(GLbitfield bit);

/// GLbitfield -> GLenum
/// @note @p bit must not be OR-ed.
GLenum
shader_bit_type(GLbitfield bit);

/// enum class ShaderStage -> GLenum
GLenum
shader_stage_type(ShaderStage stage);

/// enum class ShaderStage -> GLbitfield
GLbitfield
shader_stage_bit(ShaderStage stage);


//endregion

const char*
nameOfProgramInterface(GLenum interface);

} // namespace OpenGL

