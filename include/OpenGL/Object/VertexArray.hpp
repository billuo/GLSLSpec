/**
 * @file OpenGL/VertexArray.hpp
 * @brief Encapsulates OpenGL vertex array obejcts.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "OpenGL/Object/Object.hpp"


namespace OpenGL {

class VertexArray : public Object {
    static auto pool()
    {
        static auto singleton = makeNamePool(glGenVertexArrays, glDeleteVertexArrays);
        return singleton;
    }

  public:
    static void Bind(const VertexArray& vao)
    {
        glBindVertexArray(vao.name());
    }

    explicit VertexArray(const GLchar* label = nullptr) : Object(pool().get())
    {
        Bind(*this);
        if (label) {
            Object::label(label, GL_VERTEX_ARRAY);
        }
    }

    VertexArray(VertexArray&& obj) = default;
    VertexArray& operator=(VertexArray&& rhs) = default;

    ~VertexArray()
    { pool().put(std::move(m_name)); }

};

} // namespace OpenGL

