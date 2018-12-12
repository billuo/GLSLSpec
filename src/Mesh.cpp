/**
 * @File Mesh.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Mesh.hpp>
#include <Utility/Log.hpp>
#include <Utility/Debug.hpp>


void
Mesh::draw(GLuint VAO, GLenum layout)
{
    glBindVertexArray(VAO);
    // [0] in vec3 pos
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glBindVertexBuffer(0, m_buffer_vertex, 0, sizeof(Vertex));
    // [1] in vec3 normal
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);
    glBindVertexBuffer(1, m_buffer_normal, 0, sizeof(Normal));
    // draw
    glDrawArrays(layout, 0, m_n_vertices);
}

void
Mesh::initData(GLsizei n_vertices)
{
    if (m_n_vertices) {
        Log::e("Mesh already allocated");
    } else {
        m_n_vertices = n_vertices;
        aux_initBuffers();
    }
}

/// map/unmap vertex buffer
void*
Mesh::mapBufferVertex()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
    void* ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    return ret;
}

bool
Mesh::unmapBufferVertex()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
    GLboolean result = glUnmapBuffer(GL_ARRAY_BUFFER);
    if (result == GL_FALSE) {
        Log::w("Failed to unmap vertex buffer, data may need to mapped again.");
    }
    return result == GL_TRUE;
}

/// map/unmap normal buffer
void*
Mesh::mapBufferNormal()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_normal);
    void* ret = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    return ret;
}

bool
Mesh::unmapBufferNormal()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_normal);
    GLboolean result = glUnmapBuffer(GL_ARRAY_BUFFER);
    if (result == GL_FALSE) {
        Log::w("Failed to unmap normal buffer, data may need to mapped again.");
    }
    return result == GL_TRUE;
}
