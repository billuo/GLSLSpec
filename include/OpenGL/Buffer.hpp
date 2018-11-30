/**
 * @file OpenGL/Buffer.hpp
 * @brief Encapsulates OpenGL buffer objects.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef OPENGL_LAB_BUFFER_HPP
#define OPENGL_LAB_BUFFER_HPP
#pragma once

#include "OpenGL/Object.hpp"


namespace OpenGL {

class Buffer : public Object {

    static decltype(MakeNamePool(glCreateBuffers, glDeleteBuffers)) Pool;

  public:
    static void bind(GLenum target, Buffer& buffer)
    { glBindBuffer(target, buffer.m_name.get()); }

    static void unbind(GLenum target)
    { glBindBuffer(target, 0); }

  public:
    explicit Buffer(const GLchar* label = nullptr) : Object(Pool.Get(), label, GL_BUFFER)
    {}

    Buffer(Buffer&&) = default;

    Buffer& operator=(Buffer&&) = default;

    ~Buffer()
    { Pool.Put(std::move(m_name)); }

    // void Data(GLsizeiptr size, const GLvoid* data, GLenum usage);
    // void Storage(GLsizei size, const GLvoid* data, GLbitfield flags);

    // GLvoid* Map(GLenum access)const;
    // void Unmap() const;

    // void Clear(GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);

    // void Invalidate();

    // bool Immutable() const { return m_flags & IsImmutable; }
    // bool Mapped() const { return m_flags & IsMapped; }
    // GLsizei Size() const { return m_size; }
    // bool Empty() const { return !(m_flags & IsAllocated); }

  private:
    // enum Flags : GLbitfield {
    //     IsImmutable = 0x1,
    //     IsMapped = 0x2,
    //     IsAllocated = 0x4,
    // };
    // mutable GLbitfield m_flags = 0;
    // GLsizei m_size = 0;

    // enum Parameter : GLenum {
    //     BufferAccess = GL_BUFFER_ACCESS,
    //     BufferAccessFlags = GL_BUFFER_ACCESS_FLAGS,
    //     BufferImmutableStorage = GL_BUFFER_IMMUTABLE_STORAGE,
    //     BufferMapped = GL_BUFFER_MAPPED,
    //     BufferMapLength = GL_BUFFER_MAP_LENGTH,
    //     BufferMapOffset = GL_BUFFER_MAP_OFFSET,
    //     BufferSize = GL_BUFFER_SIZE,
    //     BufferStorageFlags = GL_BUFFER_STORAGE_FLAGS,
    //     BufferUsage = GL_BUFFER_USAGE,
    // };

    // GLint aux_GetParameter(Parameter param) const;

    // bool aux_CheckMapped(bool expected) const {
    //     if (Mapped() != expected) {
    //         DEBUG("%s %s mapped", type_name<Buffer>(), Mapped() ? "already" : "not");
    //         return false;
    //     }
    //     return true;
    // }
};
} // namespace OpenGL

#endif /* end of include guard: OPENGL_LAB_BUFFER_HPP */
