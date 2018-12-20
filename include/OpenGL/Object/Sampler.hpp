/**
 * @file Sampler.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Object.hpp>


namespace OpenGL {

class Sampler : public Object {
    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenSamplers, glDeleteSamplers);
        return singleton;
    }

  public:

    Sampler() : Object(pool().get())
    {}

    ~Sampler()
    { pool().put(std::move(m_name)); }

    void bind(GLuint unit)
    { glBindSampler(unit, name()); }

    void set(GLenum pname, GLfloat param)
    { glSamplerParameterf(name(), pname, param); }

    void set(GLenum pname, GLint param)
    { glSamplerParameteri(name(), pname, param); }

    void set(GLenum pname, const GLfloat* params)
    { glSamplerParameterfv(name(), pname, params); }

    void set(GLenum pname, const GLint* params)
    { glSamplerParameterIiv(name(), pname, params); }

    void set(GLenum pname, const GLuint* params)
    { glSamplerParameterIuiv(name(), pname, params); }

};

} // namespace OpenGL

