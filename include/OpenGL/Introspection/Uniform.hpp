#pragma once

#include "Resource.hpp"
#include "OpenGL/Constants.hpp"
#include "Utility/Debug.hpp"
#include "Utility/Log.hpp"


namespace OpenGL {

namespace details {

// TODO with extension https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_gpu_shader_fp64.txt
// TODO double may as well be supported. Detect it?

template <typename To, typename ...Args> static constexpr auto all_convertible_v = (std::is_convertible_v<Args, To> &&
...);

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

template <typename ...Args>
static inline void
glUniformxx(GLint location, Args... args)
{ static_assert(1 <= sizeof...(args) && sizeof...(args) <= 4, "Invalid parameter pack size"); }

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
    static constexpr size_t n_fields = countof(fields);

    static constexpr GLenum properties[] =
            {GL_TYPE, GL_ARRAY_SIZE, GL_OFFSET, GL_BLOCK_INDEX, GL_ARRAY_STRIDE, GL_MATRIX_STRIDE, GL_IS_ROW_MAJOR,
             GL_ATOMIC_COUNTER_BUFFER_INDEX, GL_LOCATION, GL_REFERENCED_BY_VERTEX_SHADER,
             GL_REFERENCED_BY_TESS_CONTROL_SHADER, GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
             GL_REFERENCED_BY_GEOMETRY_SHADER, GL_REFERENCED_BY_FRAGMENT_SHADER, GL_REFERENCED_BY_COMPUTE_SHADER,};
    static constexpr size_t n_properties = countof(properties);

    Uniform(GLuint program, GLint index, GLchar* name, const GLint* values);

    friend std::ostream& operator<<(std::ostream& os, const Uniform& uniform);

    /// assign this uniform with a list of values
    template <typename ...Args>
    void assign(Args... args) const noexcept
    {
        static_assert((details::all_GL_type_v<Args...>), "T must be convertible to GLfloat, GLint or GLuint.");
        // TODO check type & size against recored data type
        details::glUniformxx(location, std::forward<Args>(args)...);
    }

    /// assign this uniform with a pointer
    // TODO consider matrix
    template <typename T>
    void assign(GLsizei count, const T* x) const noexcept
    {
        static_assert(details::is_GL_type_v<T>, "T must be convertible to GLfloat, GLint or GLuint.");
        // TODO
    }

    friend struct UniformBlock;
};

} // namespace OpenGL

