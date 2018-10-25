#include "Mesh.hpp"

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
