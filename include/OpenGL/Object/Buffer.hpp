/**
 * @File Buffer.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Object.hpp>


namespace OpenGL {

class Buffer : public Object {

    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenBuffers, glDeleteBuffers);
        return singleton;
    }

  public:
    static void Bind(GLenum target, const Buffer& buffer)
    { glBindBuffer(target, buffer.m_name.get()); }

    static void Unbind(GLenum target)
    { glBindBuffer(target, 0); }

    /// @brief Allocate data store using given data usage.
    /// @param size Size of the data store in bytes.
    /// @param data Address of the initial data if any, can be nullptr.
    /// @param usage Usage of the data. GL_(STREAM|STATIC|DYNAMIC)_(DRAW|READ|COPY)
    /// @sa glBufferData()
    static void Data(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
    { glBufferData(target, size, data, usage); }

    static void Update(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data = nullptr)
    { glBufferSubData(target, offset, size, data); }

    static void Clear(GLenum target, GLenum internal_format, GLenum format, GLenum type, const GLvoid* data)
    { glClearBufferData(target, internal_format, format, type, data); }

    /// @brief Map the buffer data with given access.
    /// @param target The target to which the buffer to map is bound.
    /// @param access Access policy. GL_(READ_ONLY|WRITE_ONLY|READ_WRITE)
    /// @return A pointer the user can read from or write to based on @p access.
    static void* Map(GLenum target, GLenum access)
    { return glMapBuffer(target, access); }

    /// @brief Map a range of buffer with some hints.
    /// @param target The target to which the buffer to map is bound.
    /// @param offset Offset of the first byte to map from the beginning of buffer storage.
    /// @param length Number of the bytes to map.
    /// @param access Access policy. GL_MAP_(READ|WRITE|PERSISTENT|COHERENT)_BIT
    /// @return A pointer application can read from or write to based on @p access.
    static void* MapRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    { return glMapBufferRange(target, offset, length, access); }

    /// @brief Unmap the buffer data.
    /// @param target The target to which the buffer to map is bound.
    static void Unmap(GLenum target)
    { glUnmapBuffer(target); }

    Buffer() : Object(pool().get())
    {}

    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    ~Buffer()
    { pool().put(std::move(m_name)); }

    using Object::label;

    void label(const GLchar* label)
    { Object::label(label, GL_BUFFER); }

    void bind(GLenum target) const
    { Bind(target, *this); }

    /// @brief Invalidate the whole data store.
    void invalidate()
    { glInvalidateBufferData(name()); }

    /// @brief Partially invalidate the data store.
    /// @param offset Offset of the first byte to invalidate in the data store.
    /// @param length Number of bytes to invalidate.
    void invalidate(GLintptr offset, GLsizeiptr length)
    { glInvalidateBufferSubData(name(), offset, length); }

    GLint get(GLenum param) const;

  private:
};

} // namespace OpenGL

