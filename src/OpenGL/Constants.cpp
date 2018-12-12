/**
 * @File Constants.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Constants.hpp>
#include <Utility/Debug.hpp>
#include <Utility/Expected.hpp>
#include <Math/Math.hpp>


using namespace glm;

namespace OpenGL {

GLsizei
sizeOfDataType(GLenum type)
{
    switch (type) {
        case GL_FLOAT:
            return sizeof(float);
        case GL_FLOAT_VEC2:
            return sizeof(vec2);
        case GL_FLOAT_VEC3:
            return sizeof(vec3);
        case GL_FLOAT_VEC4:
            return sizeof(vec4);
        case GL_INT:
            return sizeof(int);
        case GL_INT_VEC2:
            return sizeof(ivec2);
        case GL_INT_VEC3:
            return sizeof(ivec3);
        case GL_INT_VEC4:
            return sizeof(ivec4);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_UNSIGNED_INT_VEC2:
            return sizeof(uvec2);
        case GL_UNSIGNED_INT_VEC3:
            return sizeof(uvec3);
        case GL_UNSIGNED_INT_VEC4:
            return sizeof(uvec4);
        case GL_BOOL:
            return sizeof(bool);
        case GL_BOOL_VEC2:
            return sizeof(bvec2);
        case GL_BOOL_VEC3:
            return sizeof(bvec3);
        case GL_BOOL_VEC4:
            return sizeof(bvec4);
        case GL_FLOAT_MAT2:
            return sizeof(mat2x2);
        case GL_FLOAT_MAT3:
            return sizeof(mat3x3);
        case GL_FLOAT_MAT4:
            return sizeof(mat4x4);
        case GL_FLOAT_MAT2x3:
            return sizeof(mat2x3);
        case GL_FLOAT_MAT2x4:
            return sizeof(mat2x4);
        case GL_FLOAT_MAT3x2:
            return sizeof(mat3x2);
        case GL_FLOAT_MAT3x4:
            return sizeof(mat3x4);
        case GL_FLOAT_MAT4x2:
            return sizeof(mat4x2);
        case GL_FLOAT_MAT4x3:
            return sizeof(mat4x3);
            // TODO double floating point not yet fully supported
        case GL_DOUBLE:
//            return sizeof(double);
        case GL_DOUBLE_VEC2:
//            return sizeof(dvec2);
        case GL_DOUBLE_VEC3:
//            return sizeof(dvec3);
        case GL_DOUBLE_VEC4:
//            return sizeof(dvec4);
        case GL_DOUBLE_MAT2:
//            return sizeof(dmat2x2);
        case GL_DOUBLE_MAT3:
//            return sizeof(dmat3x3);
        case GL_DOUBLE_MAT4:
//            return sizeof(dmat4x4);
        case GL_DOUBLE_MAT2x3:
//            return sizeof(dmat2x3);
        case GL_DOUBLE_MAT2x4:
//            return sizeof(dmat2x4);
        case GL_DOUBLE_MAT3x2:
//            return sizeof(dmat3x2);
        case GL_DOUBLE_MAT3x4:
//            return sizeof(dmat3x4);
        case GL_DOUBLE_MAT4x2:
//            return sizeof(dmat4x2);
        case GL_DOUBLE_MAT4x3:
//            return sizeof(dmat4x3);
        default:
            UNREACHABLE;
    }
}

GLsizei
numelOfDataType(GLenum type)
{
    switch (type) {
        case GL_FLOAT:
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_DOUBLE:
        case GL_BOOL:
            return 1;
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
        case GL_UNSIGNED_INT_VEC2:
        case GL_DOUBLE_VEC2:
        case GL_BOOL_VEC2:
            return 2;
        default:
            UNREACHABLE;
    }
}

const char*
nameOfDataType(GLenum type)
{
    switch (type) {
        case GL_FLOAT:
            return "float";
        case GL_FLOAT_VEC2:
            return "vec2";
        case GL_FLOAT_VEC3:
            return "vec3";
        case GL_FLOAT_VEC4:
            return "vec4";
        case GL_DOUBLE:
            return "double";
        case GL_DOUBLE_VEC2:
            return "dvec2";
        case GL_DOUBLE_VEC3:
            return "dvec3";
        case GL_DOUBLE_VEC4:
            return "dvec4";
        case GL_INT:
            return "int";
        case GL_INT_VEC2:
            return "ivec2";
        case GL_INT_VEC3:
            return "ivec3";
        case GL_INT_VEC4:
            return "ivec4";
        case GL_UNSIGNED_INT:
            return "unsigned int";
        case GL_UNSIGNED_INT_VEC2:
            return "uvec2";
        case GL_UNSIGNED_INT_VEC3:
            return "uvec3";
        case GL_UNSIGNED_INT_VEC4:
            return "uvec4";
        case GL_BOOL:
            return "bool";
        case GL_BOOL_VEC2:
            return "bvec2";
        case GL_BOOL_VEC3:
            return "bvec3";
        case GL_BOOL_VEC4:
            return "bvec4";
        case GL_FLOAT_MAT2:
            return "mat2";
        case GL_FLOAT_MAT3:
            return "mat3";
        case GL_FLOAT_MAT4:
            return "mat4";
        case GL_FLOAT_MAT2x3:
            return "mat2x3";
        case GL_FLOAT_MAT2x4:
            return "mat2x4";
        case GL_FLOAT_MAT3x2:
            return "mat3x2";
        case GL_FLOAT_MAT3x4:
            return "mat3x4";
        case GL_FLOAT_MAT4x2:
            return "mat4x2";
        case GL_FLOAT_MAT4x3:
            return "mat4x3";
        case GL_SAMPLER_1D:
            return "sampler1D";
        case GL_SAMPLER_2D:
            return "sampler2D";
        case GL_SAMPLER_3D:
            return "sampler3D";
        case GL_SAMPLER_CUBE:
            return "samplerCube";
        case GL_SAMPLER_1D_SHADOW:
            return "sampler1DShadow";
        case GL_SAMPLER_2D_SHADOW:
            return "sampler2DShadow";
        case GL_SAMPLER_1D_ARRAY:
            return "sampler1DArray";
        case GL_SAMPLER_2D_ARRAY:
            return "sampler2DArray";
        case GL_SAMPLER_1D_ARRAY_SHADOW:
            return "sampler1DArrayShadow";
        case GL_SAMPLER_2D_ARRAY_SHADOW:
            return "sampler2DArrayShadow";
        case GL_SAMPLER_2D_MULTISAMPLE:
            return "sampler2DMS";
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
            return "sampler2DMSArray";
        case GL_SAMPLER_CUBE_SHADOW:
            return "samplerCubeShadow";
        case GL_SAMPLER_BUFFER:
            return "samplerBuffer";
        case GL_SAMPLER_2D_RECT:
            return "sampler2DRect";
        case GL_SAMPLER_2D_RECT_SHADOW:
            return "sampler2DRectShadow";
        case GL_INT_SAMPLER_1D:
            return "isampler1D";
        case GL_INT_SAMPLER_2D:
            return "isampler2D";
        case GL_INT_SAMPLER_3D:
            return "isampler3D";
        case GL_INT_SAMPLER_CUBE:
            return "isamplerCube";
        case GL_INT_SAMPLER_1D_ARRAY:
            return "isampler1DArray";
        case GL_INT_SAMPLER_2D_ARRAY:
            return "isampler2DArray";
        case GL_INT_SAMPLER_2D_MULTISAMPLE:
            return "isampler2DMS";
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
            return "isampler2DMSArray";
        case GL_INT_SAMPLER_BUFFER:
            return "isamplerBuffer";
        case GL_INT_SAMPLER_2D_RECT:
            return "isampler2DRect";
        case GL_UNSIGNED_INT_SAMPLER_1D:
            return "usampler1D";
        case GL_UNSIGNED_INT_SAMPLER_2D:
            return "usampler2D";
        case GL_UNSIGNED_INT_SAMPLER_3D:
            return "usampler3D";
        case GL_UNSIGNED_INT_SAMPLER_CUBE:
            return "usamplerCube";
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
            return "usampler1DArray";
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
            return "usampler2DArray";
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
            return "usampler2DMS";
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
            return "usampler2DMSArray";
        case GL_UNSIGNED_INT_SAMPLER_BUFFER:
            return "usamplerBuffer";
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
            return "usampler2DRect";
            // TODO double floating point not yet fully supported
        case GL_DOUBLE_MAT2:
//            return "dmat2";
        case GL_DOUBLE_MAT3:
//            return "dmat3";
        case GL_DOUBLE_MAT4:
//            return "dmat4";
        case GL_DOUBLE_MAT2x3:
//            return "dmat2x3";
        case GL_DOUBLE_MAT2x4:
//            return "dmat2x4";
        case GL_DOUBLE_MAT3x2:
//            return "dmat3x2";
        case GL_DOUBLE_MAT3x4:
//            return "dmat3x4";
        case GL_DOUBLE_MAT4x2:
//            return "dmat4x2";
        case GL_DOUBLE_MAT4x3:
//            return "dmat4x3";
        default:
            UNREACHABLE;
    }
}

expected<GLenum, std::string> // suffix is expected to be small; no reference needed
shaderTypeOfSuffix(std::string suffix)
{
    static constexpr struct {
        const char* const s;
        GLenum t;
    } pairs[] =
            {
                    {"vert", GL_VERTEX_SHADER},
                    {"vs",   GL_VERTEX_SHADER},
                    {"tesc", GL_TESS_CONTROL_SHADER},
                    {"tcs",  GL_TESS_CONTROL_SHADER},
                    {"tese", GL_TESS_EVALUATION_SHADER},
                    {"tes",  GL_TESS_EVALUATION_SHADER},
                    {"geom", GL_GEOMETRY_SHADER},
                    {"gs",   GL_GEOMETRY_SHADER},
                    {"frag", GL_FRAGMENT_SHADER},
                    {"fs",   GL_FRAGMENT_SHADER},
                    {"comp", GL_COMPUTE_SHADER},
                    {"cs",   GL_COMPUTE_SHADER},
            };
    for (auto&&[s, type] : pairs) {
        if (s == suffix) {
            return type;
        }
    }
    return make_unexpected("Unknown suffix ." + suffix);
}

const char*
nameOfShaderType(GLenum type)
{
    switch (type) {
        case GL_VERTEX_SHADER:
            return "vertex shader";
        case GL_TESS_CONTROL_SHADER:
            return "tessellation control shader";
        case GL_TESS_EVALUATION_SHADER:
            return "tessellation evaluation shader";
        case GL_GEOMETRY_SHADER:
            return "geometry shader";
        case GL_FRAGMENT_SHADER:
            return "fragment shader";
        case GL_COMPUTE_SHADER:
            return "compute shader";
        default:
            UNREACHABLE;
    }
}

const char*
nameOfProgramInterface(GLenum interface)
{
    switch (interface) {
        case GL_UNIFORM:
            return "uniform";
        case GL_UNIFORM_BLOCK:
            return "uniform block";
        case GL_ATOMIC_COUNTER_BUFFER:
            return "atomic counter buffer";
        case GL_PROGRAM_INPUT:
            return "program input";
        case GL_PROGRAM_OUTPUT:
            return "program output";
        case GL_TRANSFORM_FEEDBACK_VARYING:
            return "transform feedback varying";
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            return "transform feedback buffer";
        case GL_BUFFER_VARIABLE:
            return "buffer variable";
        case GL_SHADER_STORAGE_BLOCK:
            return "shader storage block";
        case GL_VERTEX_SUBROUTINE:
            return "vertex shader subroutine";
        case GL_GEOMETRY_SUBROUTINE:
            return "geometry shader subroutine";
        case GL_FRAGMENT_SUBROUTINE:
            return "fragment shader subroutine";
        case GL_TESS_CONTROL_SUBROUTINE:
            return "tessellation control shader subroutine";
        case GL_TESS_EVALUATION_SUBROUTINE:
            return "tessellation evaluation shader subroutine";
        case GL_COMPUTE_SUBROUTINE:
            return "compute shader subroutine";
        case GL_VERTEX_SUBROUTINE_UNIFORM:
            return "vertex shader subroutine";
        case GL_GEOMETRY_SUBROUTINE_UNIFORM:
            return "geometry shader subroutine uniform";
        case GL_FRAGMENT_SUBROUTINE_UNIFORM:
            return "fragment shader subroutine uniform";
        case GL_TESS_CONTROL_SUBROUTINE_UNIFORM:
            return "tessellation control shader subroutine uniform";
        case GL_TESS_EVALUATION_SUBROUTINE_UNIFORM:
            return "tessellation evaluation shader subroutine uniform";
        case GL_COMPUTE_SUBROUTINE_UNIFORM:
            return "compute shader subroutine uniform";
        default:
            UNREACHABLE;
    }
}

GLenum
bitOfShaderType(GLenum type)
{
    switch (type) {
        case GL_VERTEX_SHADER:
            return GL_VERTEX_SHADER_BIT;
        case GL_TESS_CONTROL_SHADER:
            return GL_TESS_CONTROL_SHADER_BIT;
        case GL_TESS_EVALUATION_SHADER:
            return GL_TESS_EVALUATION_SHADER_BIT;
        case GL_GEOMETRY_SHADER:
            return GL_GEOMETRY_SHADER_BIT;
        case GL_FRAGMENT_SHADER:
            return GL_FRAGMENT_SHADER_BIT;
        case GL_COMPUTE_SHADER:
            return GL_COMPUTE_SHADER_BIT;
        default:
            UNREACHABLE;
    }
}

size_t
stageOfShaderBit(GLbitfield bits)
// TODO input is bit mask! it doesn't work generally.
{
    switch (bits) {
        case GL_VERTEX_SHADER_BIT:
            return VertexShader;
        case GL_TESS_CONTROL_SHADER_BIT:
            return TessellationControlShader;
        case GL_TESS_EVALUATION_SHADER_BIT:
            return TessellationEvaluationShader;
        case GL_GEOMETRY_SHADER_BIT:
            return GeometryShader;
        case GL_FRAGMENT_SHADER_BIT:
            return FragmentShader;
        case GL_COMPUTE_SHADER_BIT:
            return ComputeShader;
        case GL_ALL_SHADER_BITS:
            return -1;
        default:
            UNREACHABLE;
    }
}

} // namespace OpenGL
