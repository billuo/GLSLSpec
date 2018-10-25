#ifndef MESH_HPP_OXR27HFE
#define MESH_HPP_OXR27HFE
#pragma once

#include "Debug.hpp"
#include "Math.hpp"
#include "OpenGL/Object.hpp"

class Mesh {
public:
    using Vertex = glm::vec3;
    using Normal = glm::vec3;

    Mesh() : m_n_vertices(0), m_buffer_vertex(0), m_buffer_normal(0) {}

    explicit Mesh(size_t n_vertices) : m_n_vertices(n_vertices) { aux_InitBuffers(); }

    void InitData(size_t n_vertices) {
        if (m_n_vertices) {
            DEBUG("Mesh already allocated");
        } else {
            m_n_vertices = n_vertices;
            aux_InitBuffers();
        }
    }

    /// map/unmap vertex buffer
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

    /// map/unmap normal buffer
    void* MapBufferNormal() {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_normal);
        void* ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        return ret;
    }
    bool UnmapBufferNormal() {
        GLboolean result = glUnmapNamedBuffer(m_buffer_normal);
        if (result == GL_FALSE) {
            DEBUG("Failed to unmap normal buffer, data may need to mapped again.");
        }
        return result == GL_TRUE;
    }

    void Draw(GLuint VAO, GLenum layout);
    ~Mesh() { glDeleteBuffers(1, &m_buffer_vertex); }

private:
    void aux_InitBuffers() {
        assert(m_n_vertices);
        //
        glCreateBuffers(1, &m_buffer_vertex);
        assert(m_buffer_vertex);
        glNamedBufferStorage(m_buffer_vertex, m_n_vertices * sizeof(Vertex), nullptr, GL_MAP_WRITE_BIT);
        //
        glCreateBuffers(1, &m_buffer_normal);
        assert(m_buffer_vertex);
        glNamedBufferStorage(m_buffer_normal, m_n_vertices * sizeof(Normal), nullptr, GL_MAP_WRITE_BIT);
    }
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    size_t m_n_vertices;
    GLuint m_buffer_vertex;
    GLuint m_buffer_normal;
};

#endif /* end of include guard: MESH_HPP_OXR27HFE */
