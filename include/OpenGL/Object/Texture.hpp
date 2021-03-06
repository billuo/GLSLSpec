/**
 * @file Texture.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

#include "Object.hpp"


namespace OpenGL {

class Texture : public Object {
    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenTextures, glDeleteTextures);
        return singleton;
    }

  public:
    /// @brief Allocate storage for all mipmap levels of the same texture object at once.
    /// @param target The target to which the texture to allocate storage is bound.
    /// @param levels Number of mipmap levels.
    /// @param internal_format Format to store texture data inside OpenGL.
    /// @param width Size on each dimension.
    static void Storage(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width)
    { glTexStorage1D(target, levels, internal_format, width); }

    /// @brief Allocate storage for all mipmap levels of the same texture object at once.
    /// @param target The target to which the texture to allocate storage is bound.
    /// @param levels Number of mipmap levels.
    /// @param internal_format Format to store texture data inside OpenGL.
    /// @param width Width of texture
    /// @param height Height of texture, or length of array
    static void Storage(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height)
    { glTexStorage2D(target, levels, internal_format, width, height); }

    /// @brief Allocate storage for all mipmap levels of the same texture object at once.
    /// @param target The target to which the texture to allocate storage is bound.
    /// @param levels Number of mipmap levels.
    /// @param internal_format Format to store texture data inside OpenGL.
    /// @param width Width of texture in texels.
    /// @param height Height of texture in texels.
    /// @param depth Depth of texture in texels, or length of array
    static void
    Storage(GLenum target, GLsizei levels, GLenum internal_format, GLsizei width, GLsizei height, GLsizei depth)
    { glTexStorage3D(target, levels, internal_format, width, height, depth); }

    /// Represents a pair of offset and size on a specific dimension.
    using Range = std::pair<GLint, GLsizei>;

    /// @brief Specify the data of a single level of image.
    /// @param target The target to which the texture to specify is bound.
    /// @param format Format of the texel data in client memory.
    /// @param type Data type of the texel data in client memory.
    /// @param data Data to substitute with.
    /// @param level The number of mipmap level to specify.
    /// @param x x_offset and width.
    /// @note @p data can also be interpreted as offset in bytes to the beginning of the buffer bound to GL_PIXEL_UNPACK_BUFFER.
    static void
    SubImage(GLenum target, GLenum format, GLenum type, const GLvoid* data, GLint level, Range x)
    { glTexSubImage1D(target, level, x.first, x.second, format, type, data); }

    /// @brief Specify the data of a single level of image.
    /// @param target The target to which the texture to specify is bound.
    /// @param format Format of the texel data in client memory.
    /// @param type Data type of the texel data in client memory.
    /// @param data Data to substitute with.
    /// @param level The number of mipmap level to specify.
    /// @param x x_offset and width.
    /// @param y y_offset and height.
    /// @note @p data can also be interpreted as offset in bytes to the beginning of the buffer bound to GL_PIXEL_UNPACK_BUFFER.
    static void
    SubImage(GLenum target, GLenum format, GLenum type, const GLvoid* data, GLint level, Range x, Range y)
    { glTexSubImage2D(target, level, x.first, y.first, x.second, y.second, format, type, data); }

    /// @brief Specify the data of a single level of image.
    /// @param target The target to which the texture to specify is bound.
    /// @param format Format of the texel data in client memory.
    /// @param type Data type of the texel data in client memory.
    /// @param data Data to substitute with.
    /// @param level The number of mipmap level to specify.
    /// @param x x_offset and width.
    /// @param y y_offset and height.
    /// @param z z_offset and depth.
    /// @note @p data can also be interpreted as offset in bytes to the beginning of the buffer bound to GL_PIXEL_UNPACK_BUFFER.
    static void
    SubImage(GLenum target, GLenum format, GLenum type, const GLvoid* data, GLint level, Range x, Range y, Range z)
    { glTexSubImage3D(target, level, x.first, y.first, z.first, x.second, y.second, z.second, format, type, data); }

    /// @brief Activate texture unit @p unit
    /// @param unit The index of the unit to activate.
    static void
    Activate(GLuint unit)
    { glActiveTexture(GL_TEXTURE0 + unit); }

    explicit Texture() : Object(pool().get())
    {}

    // XXX no explicit
    Texture(Empty) : Object(Name(0))
    {}

    Texture(Texture&&) = default;
    Texture& operator=(Texture&&) = default;

    ~Texture()
    { pool().put(std::move(m_name)); }

    void bind(GLenum target)
    { glBindTexture(target, name()); }

    void set(GLenum pname, GLfloat param)
    { glTexParameterf(name(), pname, param); }

    void set(GLenum pname, GLint param)
    { glTexParameteri(name(), pname, param); }

    void set(GLenum pname, const GLfloat* params)
    { glTexParameterfv(name(), pname, params); }

    void set(GLenum pname, const GLint* params)
    { glTexParameterIiv(name(), pname, params); }

    void set(GLenum pname, const GLuint* params)
    { glTexParameterIuiv(name(), pname, params); }

    void storage(GLenum pname, GLint param)
    { glPixelStorei(pname, param); }

    void storage(GLenum pname, GLfloat param)
    { glPixelStoref(pname, param); }
};

} // namespace OpenGL

