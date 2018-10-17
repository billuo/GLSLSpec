#include "OpenGL/Common.hpp"
#include "Math.hpp"

namespace OpenGL {

GLsizei TypeSize(GLenum type) {
    switch (type) {
    case GL_FLOAT: return sizeof(float);
    case GL_FLOAT_VEC2: return sizeof(glm::vec2);
    case GL_FLOAT_VEC3: return sizeof(glm::vec3);
    case GL_FLOAT_VEC4: return sizeof(glm::vec4);
    case GL_DOUBLE: return sizeof(double);
    case GL_DOUBLE_VEC2: return sizeof(glm::dvec2);
    case GL_DOUBLE_VEC3: return sizeof(glm::dvec3);
    case GL_DOUBLE_VEC4: return sizeof(glm::dvec4);
    case GL_INT: return sizeof(int);
    case GL_INT_VEC2: return sizeof(glm::ivec2);
    case GL_INT_VEC3: return sizeof(glm::ivec3);
    case GL_INT_VEC4: return sizeof(glm::ivec4);
    case GL_UNSIGNED_INT: return sizeof(unsigned int);
    case GL_UNSIGNED_INT_VEC2: return sizeof(glm::uvec2);
    case GL_UNSIGNED_INT_VEC3: return sizeof(glm::uvec3);
    case GL_UNSIGNED_INT_VEC4: return sizeof(glm::uvec4);
    case GL_BOOL: return sizeof(bool);
    case GL_BOOL_VEC2: return sizeof(glm::bvec2);
    case GL_BOOL_VEC3: return sizeof(glm::bvec3);
    case GL_BOOL_VEC4: return sizeof(glm::bvec4);
    case GL_FLOAT_MAT2: return sizeof(glm::mat2);
    case GL_FLOAT_MAT3: return sizeof(glm::mat3);
    case GL_FLOAT_MAT4: return sizeof(glm::mat4);
    case GL_FLOAT_MAT2x3: return sizeof(glm::mat2x3);
    case GL_FLOAT_MAT2x4: return sizeof(glm::mat2x4);
    case GL_FLOAT_MAT3x2: return sizeof(glm::mat3x2);
    case GL_FLOAT_MAT3x4: return sizeof(glm::mat3x4);
    case GL_FLOAT_MAT4x2: return sizeof(glm::mat4x2);
    case GL_FLOAT_MAT4x3: return sizeof(glm::mat4x3);
    case GL_DOUBLE_MAT2: return sizeof(glm::dmat2);
    case GL_DOUBLE_MAT3: return sizeof(glm::dmat3);
    case GL_DOUBLE_MAT4: return sizeof(glm::dmat4);
    case GL_DOUBLE_MAT2x3: return sizeof(glm::dmat2x3);
    case GL_DOUBLE_MAT2x4: return sizeof(glm::dmat2x4);
    case GL_DOUBLE_MAT3x2: return sizeof(glm::dmat3x2);
    case GL_DOUBLE_MAT3x4: return sizeof(glm::dmat3x4);
    case GL_DOUBLE_MAT4x2: return sizeof(glm::dmat4x2);
    case GL_DOUBLE_MAT4x3: return sizeof(glm::dmat4x3);
    default: assert(false && "Unknown type"); return 0;
    }
}

const char* TypeString(GLenum type) {
    switch (type) {
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_DOUBLE: return "double";
    case GL_DOUBLE_VEC2: return "dvec2";
    case GL_DOUBLE_VEC3: return "dvec3";
    case GL_DOUBLE_VEC4: return "dvec4";
    case GL_INT: return "int";
    case GL_INT_VEC2: return "ivec2";
    case GL_INT_VEC3: return "ivec3";
    case GL_INT_VEC4: return "ivec4";
    case GL_UNSIGNED_INT: return "unsigned int";
    case GL_UNSIGNED_INT_VEC2: return "uvec2";
    case GL_UNSIGNED_INT_VEC3: return "uvec3";
    case GL_UNSIGNED_INT_VEC4: return "uvec4";
    case GL_BOOL: return "bool";
    case GL_BOOL_VEC2: return "bvec2";
    case GL_BOOL_VEC3: return "bvec3";
    case GL_BOOL_VEC4: return "bvec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_FLOAT_MAT2x3: return "mat2x3";
    case GL_FLOAT_MAT2x4: return "mat2x4";
    case GL_FLOAT_MAT3x2: return "mat3x2";
    case GL_FLOAT_MAT3x4: return "mat3x4";
    case GL_FLOAT_MAT4x2: return "mat4x2";
    case GL_FLOAT_MAT4x3: return "mat4x3";
    case GL_DOUBLE_MAT2: return "dmat2";
    case GL_DOUBLE_MAT3: return "dmat3";
    case GL_DOUBLE_MAT4: return "dmat4";
    case GL_DOUBLE_MAT2x3: return "dmat2x3";
    case GL_DOUBLE_MAT2x4: return "dmat2x4";
    case GL_DOUBLE_MAT3x2: return "dmat3x2";
    case GL_DOUBLE_MAT3x4: return "dmat3x4";
    case GL_DOUBLE_MAT4x2: return "dmat4x2";
    case GL_DOUBLE_MAT4x3: return "dmat4x3";
    case GL_SAMPLER_1D: return "sampler1D";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_1D_SHADOW: return "sampler1DShadow";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    case GL_SAMPLER_1D_ARRAY: return "sampler1DArray";
    case GL_SAMPLER_2D_ARRAY: return "sampler2DArray";
    case GL_SAMPLER_1D_ARRAY_SHADOW: return "sampler1DArrayShadow";
    case GL_SAMPLER_2D_ARRAY_SHADOW: return "sampler2DArrayShadow";
    case GL_SAMPLER_2D_MULTISAMPLE: return "sampler2DMS";
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return "sampler2DMSArray";
    case GL_SAMPLER_CUBE_SHADOW: return "samplerCubeShadow";
    case GL_SAMPLER_BUFFER: return "samplerBuffer";
    case GL_SAMPLER_2D_RECT: return "sampler2DRect";
    case GL_SAMPLER_2D_RECT_SHADOW: return "sampler2DRectShadow";
    case GL_INT_SAMPLER_1D: return "isampler1D";
    case GL_INT_SAMPLER_2D: return "isampler2D";
    case GL_INT_SAMPLER_3D: return "isampler3D";
    case GL_INT_SAMPLER_CUBE: return "isamplerCube";
    case GL_INT_SAMPLER_1D_ARRAY: return "isampler1DArray";
    case GL_INT_SAMPLER_2D_ARRAY: return "isampler2DArray";
    case GL_INT_SAMPLER_2D_MULTISAMPLE: return "isampler2DMS";
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "isampler2DMSArray";
    case GL_INT_SAMPLER_BUFFER: return "isamplerBuffer";
    case GL_INT_SAMPLER_2D_RECT: return "isampler2DRect";
    case GL_UNSIGNED_INT_SAMPLER_1D: return "usampler1D";
    case GL_UNSIGNED_INT_SAMPLER_2D: return "usampler2D";
    case GL_UNSIGNED_INT_SAMPLER_3D: return "usampler3D";
    case GL_UNSIGNED_INT_SAMPLER_CUBE: return "usamplerCube";
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return "usampler1DArray";
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return "usampler2DArray";
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return "usampler2DMS";
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return "usampler2DMSArray";
    case GL_UNSIGNED_INT_SAMPLER_BUFFER: return "usamplerBuffer";
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return "usampler2DRect";
    default: return "UNKNOWN";
    }
}

} // namespace OpenGL
