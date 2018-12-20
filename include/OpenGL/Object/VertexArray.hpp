/**
 * @file OpenGL/VertexArray.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Object.hpp>


namespace OpenGL {

class VertexArray : public Object {
    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenVertexArrays, glDeleteVertexArrays);
        return singleton;
    }

  public:
    static void Bind(const VertexArray& vao)
    { glBindVertexArray(vao.name()); }

    explicit VertexArray() : Object(pool().get())
    {}

    VertexArray(VertexArray&& obj) = default;
    VertexArray& operator=(VertexArray&& rhs) = default;

    ~VertexArray()
    { pool().put(std::move(m_name)); }

    using Object::label;

    VertexArray& label(const GLchar* label)
    {
        Object::label(label, GL_VERTEX_ARRAY);
        return *this;
    }

    const VertexArray& bind() const
    {
        Bind(*this);
        return *this;
    }

    VertexArray& bind()
    {
        Bind(*this);
        return *this;
    }

};

} // namespace OpenGL

