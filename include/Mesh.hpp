#ifndef MESH_HPP_OXR27HFE
#define MESH_HPP_OXR27HFE
#pragma once

#include "Debug.hpp"
#include "Math.hpp"
#include "OpenGL/Object.hpp"

class Mesh {
public:
    typedef glm::vec3 Vertex;

    Mesh() : m_n_vertices(0), m_buffer_vertex(0) {}

    explicit Mesh(size_t n_vertices) : m_n_vertices(n_vertices) {
        glCreateBuffers(1, &m_buffer_vertex);
        assert(m_buffer_vertex);
        glNamedBufferStorage(m_buffer_vertex, n_vertices * sizeof(Vertex), nullptr, GL_MAP_WRITE_BIT);
    }

    void InitData(size_t n_vertices) {
        if (m_n_vertices) {
            DEBUG("Mesh already allocated");
        } else {
            m_n_vertices = n_vertices;
            glCreateBuffers(1, &m_buffer_vertex);
            assert(glIsBuffer(m_buffer_vertex));
            glNamedBufferStorage(m_buffer_vertex, n_vertices * sizeof(Vertex), nullptr, GL_MAP_WRITE_BIT);
        }
    }

    void* MapBufferVertex() {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
        void* ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        return ret;
    }
    bool UnmapBufferVertex() {
        GLboolean result = glUnmapNamedBuffer(m_buffer_vertex);
        if (result == GL_FALSE) {
            DEBUG("Failed to unmap vertex buffer, data may need to mapped again.");
        }
        return result == GL_TRUE;
    }
    void Draw(GLuint VAO, GLenum layout) {
        glBindVertexArray(VAO);
        // [0] in vec3 pos
        glEnableVertexArrayAttrib(VAO, 0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);
        glBindVertexBuffer(0, m_buffer_vertex, 0, sizeof(Vertex));
        // [1] in vec3 normal
        glEnableVertexArrayAttrib(VAO, 1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 0); // @TODO update real normal data.
        // draw
        glDrawArrays(layout, 0, m_n_vertices);
    }
    ~Mesh() { glDeleteBuffers(1, &m_buffer_vertex); }

private:
    Mesh(const Mesh&);
    Mesh& operator=(const Mesh&);

    size_t m_n_vertices;
    GLuint m_buffer_vertex;
};

#endif /* end of include guard: MESH_HPP_OXR27HFE */
