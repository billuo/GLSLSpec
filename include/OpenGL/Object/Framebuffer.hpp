/**
 * @file FrameBuffer.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

// TODO implement FBO to support postprocessing

#include "RenderBuffer.hpp"
#include "Texture.hpp"


namespace OpenGL {

class Framebuffer : public Object {
    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenFramebuffers, glDeleteFramebuffers);
        return singleton;
    }

  public:
    static void Bind(GLenum target, Framebuffer& obj)
    { glBindFramebuffer(target, obj.name()); }

    static void Unbind(GLenum target)
    { glBindFramebuffer(target, 0u); }

    static void Set(GLenum target, GLenum pname, GLint param)
    { glFramebufferParameteri(target, pname, param); }

    static GLint Get(GLenum target, GLenum pname)
    {
        GLint ret = -1;
        glGetFramebufferParameteriv(target, pname, &ret);
        return ret;
    }

    static void Attach(GLenum target, GLenum attachment, const Texture& texture, GLint level)
    { glFramebufferTexture(target, attachment, texture.name(), level); }

    static void Attach1D(GLenum target, GLenum attachment, GLenum textarget, const Texture& texture, GLint level)
    { glFramebufferTexture1D(target, attachment, textarget, texture.name(), level); }

    static void Attach2D(GLenum target, GLenum attachment, GLenum textarget, const Texture& texture, GLint level)
    { glFramebufferTexture2D(target, attachment, textarget, texture.name(), level); }

    static void
    Attach3D(GLenum target, GLenum attachment, GLenum textarget, const Texture& texture, GLint level, GLint layer)
    { glFramebufferTexture3D(target, attachment, textarget, texture.name(), level, layer); }

    explicit Framebuffer() : Object(pool().get())
    {}

    // XXX no explicit
    Framebuffer(Empty) : Object(Name(0u))
    {}

    ~Framebuffer()
    { pool().put(std::move(m_name)); }

    using Object::label;

    Framebuffer& label(const GLchar* label)
    {
        Object::label(label, GL_FRAMEBUFFER);
        return *this;
    }

    Framebuffer& bind(GLenum target)
    {
        Bind(target, *this);
        return *this;
    }

    Framebuffer& unbind(GLenum target)
    {
        Unbind(target);
        return *this;
    }
};

} // namespace OpenGL

