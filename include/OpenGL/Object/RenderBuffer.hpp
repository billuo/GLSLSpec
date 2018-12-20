/**
 * @file RenderBuffer.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Object.hpp>


namespace OpenGL {

class RenderBuffer : public Object {
    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenRenderbuffers, glDeleteRenderbuffers);
        return singleton;
    }

  public:

    static void Bind(RenderBuffer& obj)
    { glBindRenderbuffer(GL_RENDERBUFFER, obj.name()); }

    static void Unbind()
    { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

    /// @brief Specify data storage for RenderBuffer
    /// @param target
    /// @param internal_format
    /// @param width
    /// @param height
    static void Storage(GLenum internal_format, GLsizei width, GLsizei height)
    { glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height); }

    static void
    Storage(GLsizei samples, GLenum internal_format, GLsizei width, GLsizei height)
    { glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internal_format, width, height); }

    static GLint Get(GLenum pname)
    {
        GLint ret = -1;
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, pname, &ret);
        return ret;
    }

    explicit RenderBuffer() : Object(pool().get())
    {}

    // XXX no explicit
    RenderBuffer(Empty) : Object(Name(0u))
    {}

    ~RenderBuffer()
    { pool().put(std::move(m_name)); }

    void bind()
    { Bind(*this); }
};

} // namespace OpenGL

