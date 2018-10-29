#include "Mesh.hpp"
#include "Debug.hpp"

void Mesh::Draw(GLuint VAO, GLenum layout) {
    glBindVertexArray(VAO);
    // [0] in vec3 pos
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glBindVertexBuffer(0, m_buffer_vertex, 0, sizeof(Vertex));
    // [1] in vec3 normal
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);
    glBindVertexBuffer(1, m_buffer_normal, 0, sizeof(Normal));
    // draw
    glDrawArrays(layout, 0, m_n_vertices);
}

void Mesh::InitData(size_t n_vertices) {
    if (m_n_vertices) {
        DEBUG("Mesh already allocated");
    } else {
        m_n_vertices = n_vertices;
        aux_InitBuffers();
    }
}

/// map/unmap vertex buffer
void* Mesh::MapBufferVertex() {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
    void* ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    return ret;
}
bool Mesh::UnmapBufferVertex() {
    GLboolean result = glUnmapNamedBuffer(m_buffer_vertex);
    if (result == GL_FALSE) {
        DEBUG("Failed to unmap vertex buffer, data may need to mapped again.");
    }
    return result == GL_TRUE;
}

/// map/unmap normal buffer
void* Mesh::MapBufferNormal() {
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_normal);
    void* ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    return ret;
}
bool Mesh::UnmapBufferNormal() {
    GLboolean result = glUnmapNamedBuffer(m_buffer_normal);
    if (result == GL_FALSE) {
        DEBUG("Failed to unmap normal buffer, data may need to mapped again.");
    }
    return result == GL_TRUE;
}
