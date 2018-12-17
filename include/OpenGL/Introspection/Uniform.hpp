/**
 * @File Uniform.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Math/Math.hpp>
#include <OpenGL/Introspection/Resource.hpp>
#include <OpenGL/Constants.hpp>
#include <Utility/Debug.hpp>


namespace OpenGL {

namespace details {

template <typename To, typename ...Args> static constexpr auto
        all_convertible_v = (std::is_convertible_v<Args, To> && ...);

template <typename T> static constexpr auto is_GL_type_v =
        std::is_convertible_v<T, GLfloat> || std::is_convertible_v<T, GLint> || std::is_convertible_v<T, GLuint>;

template <typename ...Args> static constexpr auto all_GL_type_v =
        all_convertible_v<GLfloat, Args...> || all_convertible_v<GLint, Args...> || all_convertible_v<GLuint, Args...>;

// TODO the order of conditional may have unexpected effect on result...
template <typename ...Args> using GL_cast =
std::conditional_t<all_convertible_v<GLfloat, Args...>,
        GLfloat,
        std::conditional_t<all_convertible_v<GLint, Args...>,
                GLint,
                std::conditional_t<all_convertible_v<GLuint, Args...>, GLuint, void>>>;

//region glUniformxx

template <typename ...Args>
static inline void
glUniformxx(GLint location, Args... args)
{
    static_assert(all_GL_type_v<Args...>, "Arguments can not be converted to acceptable GL types");
    static_assert(1 <= sizeof...(args) && sizeof...(args) <= 4, "Invalid parameter pack size");
}

//region glUniform*f

template <>
void
glUniformxx<GLfloat>(GLint location, GLfloat v0)
{ return glUniform1f(location, v0); }

template <>
void
glUniformxx<GLfloat>(GLint location, GLfloat v0, GLfloat v1)
{ return glUniform2f(location, v0, v1); }

template <>
void
glUniformxx<GLfloat>(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{ return glUniform3f(location, v0, v1, v2); }

template <>
void
glUniformxx<GLfloat>(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{ return glUniform4f(location, v0, v1, v2, v3); }

//endregion

//region glUniform*i

template <>
void
glUniformxx<GLint>(GLint location, GLint v0)
{ return glUniform1i(location, v0); }

template <>
void
glUniformxx<GLint>(GLint location, GLint v0, GLint v1)
{ return glUniform2i(location, v0, v1); }

template <>
void
glUniformxx<GLint>(GLint location, GLint v0, GLint v1, GLint v2)
{ return glUniform3i(location, v0, v1, v2); }

template <>
void
glUniformxx<GLint>(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{ return glUniform4i(location, v0, v1, v2, v3); }

//endregion

//region glUniform*ui

template <>
void
glUniformxx<GLuint>(GLint location, GLuint v0)
{ return glUniform1ui(location, v0); }

template <>
void
glUniformxx<GLuint>(GLint location, GLuint v0, GLuint v1)
{ return glUniform2ui(location, v0, v1); }

template <>
void
glUniformxx<GLuint>(GLint location, GLuint v0, GLuint v1, GLuint v2)
{ return glUniform3ui(location, v0, v1, v2); }

template <>
void
glUniformxx<GLuint>(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{ return glUniform4ui(location, v0, v1, v2, v3); }

//endregion

//region glUniform*d

// TODO detect extension https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_gpu_shader_fp64.txt

template <>
void
glUniformxx<GLdouble>(GLint location, GLdouble v0)
{ return glUniform1d(location, v0); }

template <>
void
glUniformxx<GLdouble>(GLint location, GLdouble v0, GLdouble v1)
{ return glUniform2d(location, v0, v1); }

template <>
void
glUniformxx<GLdouble>(GLint location, GLdouble v0, GLdouble v1, GLdouble v2)
{ return glUniform3d(location, v0, v1, v2); }

template <>
void
glUniformxx<GLdouble>(GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
{ return glUniform4d(location, v0, v1, v2, v3); }

//endregion

template <glm::length_t L, typename T, glm::qualifier Q>
static constexpr inline void
glUniformxx(GLint location, glm::vec<L, T, Q> vec)
{
    if constexpr (std::is_same_v<T, GLfloat>) {
        switch (L) {
            case 2:
                return glUniform2f(location, vec[0], vec[1]);
            case 3:
                return glUniform3f(location, vec[0], vec[1], vec[2]);
            case 4:
                return glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    } else if constexpr (std::is_same_v<T, GLint>) {
        switch (L) {
            case 2:
                return glUniform2i(location, vec[0], vec[1]);
            case 3:
                return glUniform3i(location, vec[0], vec[1], vec[2]);
            case 4:
                return glUniform4i(location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    } else if constexpr (std::is_same_v<T, GLuint>) {
        switch (L) {
            case 2:
                return glUniform2ui(location, vec[0], vec[1]);
            case 3:
                return glUniform3ui(location, vec[0], vec[1], vec[2]);
            case 4:
                return glUniform4ui(location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    } else if constexpr (std::is_same_v<T, GLdouble>) {
        switch (L) {
            case 2:
                return glUniform2d(location, vec[0], vec[1]);
            case 3:
                return glUniform3d(location, vec[0], vec[1], vec[2]);
            case 4:
                return glUniform4d(location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    }
    UNREACHABLE;
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q,
        typename = std::enable_if_t<std::is_same_v<T, GLfloat>>>
static inline void
glUniformMatrixxfv(GLint location, GLsizei count, GLboolean transpose, const glm::mat<C, R, T, Q>& value)
{
    if constexpr (R == 2) {
        if constexpr (C == 2) {
            return glUniformMatrix2fv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glUniformMatrix2x3fv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glUniformMatrix2x4fv(location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 3) {
        if constexpr (C == 2) {
            return glUniformMatrix3x2fv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glUniformMatrix3fv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glUniformMatrix3x4fv(location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 4) {
        if constexpr (C == 2) {
            return glUniformMatrix4x2fv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glUniformMatrix4x3fv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glUniformMatrix4fv(location, count, transpose, glm::value_ptr(value));
        }
    }
    UNREACHABLE;
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q,
        typename = std::enable_if_t<std::is_same_v<T, GLdouble>>>
static inline void
glUniformMatrixxdv(GLint location, GLsizei count, GLboolean transpose, const glm::mat<C, R, T, Q>& value)
{
    if constexpr (R == 2) {
        if constexpr (C == 2) {
            return glUniformMatrix2dv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glUniformMatrix2x3dv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glUniformMatrix2x4dv(location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 3) {
        if constexpr (C == 2) {
            return glUniformMatrix3x2dv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glUniformMatrix3dv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glUniformMatrix3x4dv(location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 4) {
        if constexpr (C == 2) {
            return glUniformMatrix4x2dv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glUniformMatrix4x3dv(location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glUniformMatrix4dv(location, count, transpose, glm::value_ptr(value));
        }
    }
    UNREACHABLE;
}

//endregion

//region glProgramUniformXX

template <typename ...Args>
static inline void
glProgramUniformxx(GLuint program, GLint location, Args... args)
{
    static_assert(all_GL_type_v<Args...>, "Arguments can not be converted to acceptable GL types");
    static_assert(1 <= sizeof...(args) && sizeof...(args) <= 4, "Invalid parameter pack size");
}

//region glProgramUniform*f

template <>
void
glProgramUniformxx<GLfloat>(GLuint program, GLint location, GLfloat v0)
{ return glProgramUniform1f(program, location, v0); }

template <>
void
glProgramUniformxx<GLfloat>(GLuint program, GLint location, GLfloat v0, GLfloat v1)
{ return glProgramUniform2f(program, location, v0, v1); }

template <>
void
glProgramUniformxx<GLfloat>(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{ return glProgramUniform3f(program, location, v0, v1, v2); }

template <>
void
glProgramUniformxx<GLfloat>(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{ return glProgramUniform4f(program, location, v0, v1, v2, v3); }

//endregion

//region glProgramUniform*i

template <>
void
glProgramUniformxx<GLint>(GLuint program, GLint location, GLint v0)
{ return glProgramUniform1i(program, location, v0); }

template <>
void
glProgramUniformxx<GLint>(GLuint program, GLint location, GLint v0, GLint v1)
{ return glProgramUniform2i(program, location, v0, v1); }

template <>
void
glProgramUniformxx<GLint>(GLuint program, GLint location, GLint v0, GLint v1, GLint v2)
{ return glProgramUniform3i(program, location, v0, v1, v2); }

template <>
void
glProgramUniformxx<GLint>(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{ return glProgramUniform4i(program, location, v0, v1, v2, v3); }

//endregion

//region glProgramUniform*ui

template <>
void
glProgramUniformxx<GLuint>(GLuint program, GLint location, GLuint v0)
{ return glProgramUniform1ui(program, location, v0); }

template <>
void
glProgramUniformxx<GLuint>(GLuint program, GLint location, GLuint v0, GLuint v1)
{ return glProgramUniform2ui(program, location, v0, v1); }

template <>
void
glProgramUniformxx<GLuint>(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)
{ return glProgramUniform3ui(program, location, v0, v1, v2); }

template <>
void
glProgramUniformxx<GLuint>(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{ return glProgramUniform4ui(program, location, v0, v1, v2, v3); }

//endregion

//region glProgramUniform*d

// TODO ditto

template <>
void
glProgramUniformxx<GLdouble>(GLuint program, GLint location, GLdouble v0)
{ return glProgramUniform1d(program, location, v0); }

template <>
void
glProgramUniformxx<GLdouble>(GLuint program, GLint location, GLdouble v0, GLdouble v1)
{ return glProgramUniform2d(program, location, v0, v1); }

template <>
void
glProgramUniformxx<GLdouble>(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2)
{ return glProgramUniform3d(program, location, v0, v1, v2); }

template <>
void
glProgramUniformxx<GLdouble>(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
{ return glProgramUniform4d(program, location, v0, v1, v2, v3); }

//endregion

template <glm::length_t L, typename T, glm::qualifier Q>
static constexpr inline void
glProgramUniformxx(GLuint program, GLint location, glm::vec<L, T, Q> vec)
{
    if constexpr (std::is_same_v<T, GLfloat>) {
        switch (L) {
            case 2:
                return glProgramUniform2f(program, location, vec[0], vec[1]);
            case 3:
                return glProgramUniform3f(program, location, vec[0], vec[1], vec[2]);
            case 4:
                return glProgramUniform4f(program, location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    } else if constexpr (std::is_same_v<T, GLint>) {
        switch (L) {
            case 2:
                return glProgramUniform2i(program, location, vec[0], vec[1]);
            case 3:
                return glProgramUniform3i(program, location, vec[0], vec[1], vec[2]);
            case 4:
                return glProgramUniform4i(program, location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    } else if constexpr (std::is_same_v<T, GLuint>) {
        switch (L) {
            case 2:
                return glProgramUniform2ui(program, location, vec[0], vec[1]);
            case 3:
                return glProgramUniform3ui(program, location, vec[0], vec[1], vec[2]);
            case 4:
                return glProgramUniform4ui(program, location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    } else if constexpr (std::is_same_v<T, GLdouble>) {
        switch (L) {
            case 2:
                return glProgramUniform2d(program, location, vec[0], vec[1]);
            case 3:
                return glProgramUniform3d(program, location, vec[0], vec[1], vec[2]);
            case 4:
                return glProgramUniform4d(program, location, vec[0], vec[1], vec[2], vec[3]);
            default:
                break;
        }
    }
    UNREACHABLE;
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q,
        typename = std::enable_if_t<std::is_same_v<T, GLfloat>>>
static inline void
glProgramUniformMatrixxfv(GLuint program, GLint location, GLsizei count, GLboolean transpose,
                          const glm::mat<C, R, T, Q>& value)
{
    if constexpr (R == 2) {
        if constexpr (C == 2) {
            return glProgramUniformMatrix2fv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glProgramUniformMatrix2x3fv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glProgramUniformMatrix2x4fv(program, location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 3) {
        if constexpr (C == 2) {
            return glProgramUniformMatrix3x2fv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glProgramUniformMatrix3fv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glProgramUniformMatrix3x4fv(program, location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 4) {
        if constexpr (C == 2) {
            return glProgramUniformMatrix4x2fv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glProgramUniformMatrix4x3fv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glProgramUniformMatrix4fv(program, location, count, transpose, glm::value_ptr(value));
        }
    }
    UNREACHABLE;
}

template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q,
        typename = std::enable_if_t<std::is_same_v<T, GLdouble>>>
static inline void
glProgramUniformMatrixxdv(GLuint program, GLint location, GLsizei count, GLboolean transpose,
                          const glm::mat<C, R, T, Q>& value)
{
    if constexpr (R == 2) {
        if constexpr (C == 2) {
            return glProgramUniformMatrix2dv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glProgramUniformMatrix2x3dv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glProgramUniformMatrix2x4dv(program, location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 3) {
        if constexpr (C == 2) {
            return glProgramUniformMatrix3x2dv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glProgramUniformMatrix3dv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glProgramUniformMatrix3x4dv(program, location, count, transpose, glm::value_ptr(value));
        }
    } else if constexpr (R == 4) {
        if constexpr (C == 2) {
            return glProgramUniformMatrix4x2dv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 3) {
            return glProgramUniformMatrix4x3dv(program, location, count, transpose, glm::value_ptr(value));
        } else if constexpr (C == 4) {
            return glProgramUniformMatrix4dv(program, location, count, transpose, glm::value_ptr(value));
        }
    }
    UNREACHABLE;
}

//endregion

} // namespace details

struct Uniform : public Resource {
    static constexpr GLenum interface = GL_UNIFORM;

    GLint type = 0;
    GLint asize = 0;
    GLint offset = -1;
    GLint block_index = -1;
    GLint astride = -1;
    GLint mstride = -1;
    GLint row_major = false;
    GLint atomic_index = -1;
    GLint location = -1;
    GLint referenced[MaxShaderStage] = {};

    using GLintfield = GLint(Uniform::*);
    static constexpr GLintfield fields[] =
            {&Uniform::type, &Uniform::asize, &Uniform::offset, &Uniform::block_index, &Uniform::astride,
             &Uniform::mstride, &Uniform::row_major, &Uniform::atomic_index, &Uniform::location,};
    static constexpr size_t n_fields = numel(fields);

    static constexpr GLenum properties[] =
            {GL_TYPE, GL_ARRAY_SIZE, GL_OFFSET, GL_BLOCK_INDEX, GL_ARRAY_STRIDE, GL_MATRIX_STRIDE, GL_IS_ROW_MAJOR,
             GL_ATOMIC_COUNTER_BUFFER_INDEX, GL_LOCATION, GL_REFERENCED_BY_VERTEX_SHADER,
             GL_REFERENCED_BY_TESS_CONTROL_SHADER, GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
             GL_REFERENCED_BY_GEOMETRY_SHADER, GL_REFERENCED_BY_FRAGMENT_SHADER, GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = numel(properties);

    Uniform(GLuint program, GLint index, GLchar* name, const GLint* values);

    friend std::ostream& operator<<(std::ostream& os, const Uniform& uniform);

    /// assign this uniform with a list of values
    template <typename ...Args, typename = std::enable_if_t<details::all_GL_type_v<Args...>>>
    void assign(Args... args) const noexcept
    {
        // TODO check type & size against recorded data type
        details::glUniformxx(location, std::forward<Args>(args)...);
    }

    template <glm::length_t L, typename T, glm::qualifier Q>
    void assign(const glm::vec<L, T, Q>& vec) const noexcept
    { details::glUniformxx(location, vec); }

    template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    void assign(GLsizei count, GLboolean transpose, const glm::mat<C, R, T, Q>& mat) const noexcept
    {
        if constexpr (std::is_same_v<T, GLfloat>) {
            details::glUniformMatrixxfv(location, count, transpose, mat);
        } else if constexpr (std::is_same_v<T, GLdouble>) {
            details::glUniformMatrixxdv(location, count, transpose, mat);
        }
    }

    /// assign this uniform with a list of values
    template <typename ...Args, typename = std::enable_if_t<details::all_GL_type_v<Args...>>>
    void assign(GLuint program, Args... args) const noexcept
    {
        // TODO check type & size against recorded data type
        details::glProgramUniformxx(program, location, std::forward<Args>(args)...);
    }

    template <glm::length_t L, typename T, glm::qualifier Q>
    void assign(GLuint program, const glm::vec<L, T, Q>& vec) const noexcept
    { details::glProgramUniformxx(program, location, vec); }

    template <glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
    void assign(GLuint program, GLsizei count, GLboolean transpose, const glm::mat<C, R, T, Q>& mat) const noexcept
    {
        if constexpr (std::is_same_v<T, GLfloat>) {
            details::glProgramUniformMatrixxfv(program, location, count, transpose, mat);
        } else if constexpr (std::is_same_v<T, GLdouble>) {
            details::glProgramUniformMatrixxdv(program, location, count, transpose, mat);
        }
    }

    friend struct UniformBlock;
};

} // namespace OpenGL

