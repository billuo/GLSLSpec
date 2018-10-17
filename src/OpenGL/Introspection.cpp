#include "OpenGL/Introspection.hpp"

namespace OpenGL {

constexpr GLenum Uniform::properties[];
constexpr size_t Uniform::n_properties;
constexpr Uniform::GLintfield Uniform::fields[];
constexpr size_t Uniform::n_fields;

constexpr GLenum UniformBlock::properties[];
constexpr size_t UniformBlock::n_properties;

const char* InterfaceString(GLenum interface) {
    switch (interface) {
    case GL_UNIFORM: return "uniform";
    case GL_UNIFORM_BLOCK: return "uniform block";
    case GL_ATOMIC_COUNTER_BUFFER: return "atomic counter buffer";
    case GL_PROGRAM_INPUT: return "program input";
    case GL_PROGRAM_OUTPUT: return "program output";
    case GL_TRANSFORM_FEEDBACK_VARYING: return "transform feedback varying";
    case GL_TRANSFORM_FEEDBACK_BUFFER: return "transform feedback buffer";
    case GL_BUFFER_VARIABLE: return "buffer variable";
    case GL_SHADER_STORAGE_BLOCK: return "shader storage block";
    case GL_VERTEX_SUBROUTINE: return "vertex shader subroutine";
    case GL_GEOMETRY_SUBROUTINE: return "geometry shader subroutine";
    case GL_FRAGMENT_SUBROUTINE: return "fragment shader subroutine";
    case GL_TESS_CONTROL_SUBROUTINE: return "tessellation control shader subroutine";
    case GL_TESS_EVALUATION_SUBROUTINE: return "tessellation evaluation shader subroutine";
    case GL_COMPUTE_SUBROUTINE: return "compute shader subroutine";
    case GL_VERTEX_SUBROUTINE_UNIFORM: return "vertex shader subroutine";
    case GL_GEOMETRY_SUBROUTINE_UNIFORM: return "geometry shader subroutine uniform";
    case GL_FRAGMENT_SUBROUTINE_UNIFORM: return "fragment shader subroutine uniform";
    case GL_TESS_CONTROL_SUBROUTINE_UNIFORM: return "tessellation control shader subroutine uniform";
    case GL_TESS_EVALUATION_SUBROUTINE_UNIFORM: return "tessellation evaluation shader subroutine uniform";
    case GL_COMPUTE_SUBROUTINE_UNIFORM: return "compute shader subroutine uniform";
    default: return "UNKNOWN";
    }
}

} // namespace OpenGL
