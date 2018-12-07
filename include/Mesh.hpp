#pragma once

#include "Math.hpp"
#include "OpenGL/Object/Object.hpp"


class Mesh {
  public:
    using Vertex = glm::vec3;
    using Normal = glm::vec3;

    Mesh() noexcept : m_n_vertices(0), m_buffer_vertex(0), m_buffer_normal(0)
    {}

    Mesh(const Mesh&) = delete;

    Mesh& operator=(const Mesh&) = delete;

    // explicit Mesh(size_t n_vertices) : m_n_vertices(n_vertices) { aux_initBuffers(); }

    void initData(GLsizei n_vertices);

    /// map/unmap vertex buffer
    void* mapBufferVertex();

    bool unmapBufferVertex();

    /// map/unmap normal buffer
    void* mapBufferNormal();

    bool unmapBufferNormal();

    void draw(GLuint VAO, GLenum layout);

    ~Mesh()
    { glDeleteBuffers(1, &m_buffer_vertex); }

  private:
    void aux_initBuffers()
    {
        assert(m_n_vertices);
        //
        glGenBuffers(1, &m_buffer_vertex);
        assert(m_buffer_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
        glBufferData(GL_ARRAY_BUFFER, m_n_vertices * sizeof(Vertex), nullptr, GL_STATIC_DRAW);
        //
        glGenBuffers(1, &m_buffer_normal);
        assert(m_buffer_vertex);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_normal);
        glBufferData(GL_ARRAY_BUFFER, m_n_vertices * sizeof(Normal), nullptr, GL_MAP_WRITE_BIT);
    }

    GLsizei m_n_vertices;
    GLuint m_buffer_vertex;
    GLuint m_buffer_normal;
};

