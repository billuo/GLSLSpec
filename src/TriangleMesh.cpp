/**
 * @file TriangleMesh.cpp
 * @brief Implements constructing and drawing for TriangleMesh.
 * @author Zhen Luo 461652354@qq.com
 */
#include <algorithm>

#include "OpenGL/VertexArray.hpp"
#include "TriangleMesh.hpp"
#include "Video.hpp"

TriangleMesh::TriangleMesh(GLsizei n_vertex, GLsizei n_index /* = 0 */) :
        m_layout(GL_TRIANGLES),
        m_buffer_pos(),
        m_buffer_index(),
        m_buffer_vertex_normal() {
    m_buffer_pos.Create();
    m_buffer_vertex_normal.Create();
    /// @TODO specify tighter accesses
    m_buffer_pos.Storage(n_vertex * sizeof(Position), NULL, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    m_buffer_vertex_normal.Storage(n_vertex * sizeof(Normal), NULL, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    if (n_index) {
        m_buffer_index.Create();
        m_buffer_index.Storage(n_index * sizeof(Index), NULL, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    }
}

void TriangleMesh::Draw() {
    CHECK_OPENGL();
    Video::OpenGL::VertexArray& vao = Video::Root::GetVertexArray();
    // layout (location=0) in vec4 pos
    CHECK_OPENGL();
    vao.VertexBuffer(0, m_buffer_pos.Name(), 0, sizeof(Position));
    vao.AttribBinding(0, 0);
    vao.AttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    vao.EnableAttrib(0);
    CHECK_OPENGL();
    // layout (location=1) in vec3 normal
    // vao.VertexBuffer(1, m_buffer_vertex_normal.Name(), 0, sizeof(Normal));
    // vao.AttribBinding(1, 1);
    // vao.AttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    // vao.EnableAttrib(1);
    CHECK_OPENGL();
    // draw
    if (Indexed()) {
        m_buffer_index.Bind(GL_ELEMENT_ARRAY_BUFFER);
        glDrawElements(m_layout, m_buffer_index.Size() / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(m_layout, 0, m_buffer_pos.Size() / sizeof(Position));
    }
    CHECK_OPENGL();
}

// void TriangleMesh::aux_CalculateNormals() {
//     if (!m_vertex_normal.Empty() && !m_normal.Empty()) {
//         return; // nothing to be done
//     }
//     // first compute face normal.
//     size_t size = Triangles();
//     LogValue(size, "%lu");
//     LogValue(m_vertices.Size(), "%lu");
//     m_normal.Resize(size);
//     if (!m_vertex_normal.Empty()) { // make use of vertex normal if present
//         Debug("calculating face normal from vertex normal.");
//         for (size_t i = 0; i < size; ++i) {
//             /// @TODO what if n0, n1 and n2 are of different lengths? Bias will be introduced.
//             auto& n0 = m_vertex_normal[aux_TriangleIndex(i, 0)];
//             auto& n1 = m_vertex_normal[aux_TriangleIndex(i, 1)];
//             auto& n2 = m_vertex_normal[aux_TriangleIndex(i, 2)];
//             // m_normal[i] = normalize(n0 + n1 + n2);
//             m_normal[i] = normalize(normalize(n0) + normalize(n1) + normalize(n2)); // XXX solves todo tho at a
//             cost.
//         }
//     } else {
//         DEBUG("calculating face normal from vertices pos.");
//         m_vertex_normal.Resize(m_vertices.Size());
//         for (size_t i = 0; i < size; ++i) {
//             // we assume vertices are stored in CCS order, and its normal points towards us.
//             auto& p0 = m_vertices[aux_TriangleIndex(i, 0)];
//             auto& p1 = m_vertices[aux_TriangleIndex(i, 1)];
//             auto& p2 = m_vertices[aux_TriangleIndex(i, 2)];
//             // XXX no normalization here! So that m_normal[i] remain propotional to the area of the triangle,
//             // which in turn serves as a weight over it when calculating vertex normal.
//             m_normal[i] = cross(p1 - p0, p2 - p0);
//             // then compute vertex normal.
//             m_vertex_normal[aux_TriangleIndex(i, 0)] += m_normal[i];
//             m_vertex_normal[aux_TriangleIndex(i, 1)] += m_normal[i];
//             m_vertex_normal[aux_TriangleIndex(i, 2)] += m_normal[i];
//         }
//         for (auto& n : m_vertex_normal) {
//             n = Math::normalize(n);
//         }
//     }
// }

size_t TriangleMesh::aux_TriangleIndex(uint32_t n, uint_fast32_t p) {
    assert(p < 3);
    uint32_t index;
    switch (m_layout) {
    case GL_TRIANGLES: index = n * 3 + p; break;
    case GL_TRIANGLE_STRIP: index = n + p; break;
    case GL_TRIANGLE_FAN: index = p ? 2 * n + p : 0; break;
    default:
        DEBUG("Unknown triangle layout=%d.", m_layout);
        index = p;
        break;
    }
    return index; /// @TODO result might be index of index rather than vertices.
}

void TriangleMesh::aux_CheckSize() {
    size_t size;
    if (Indexed()) {
        size = m_buffer_pos.Size() / sizeof(Position);
    } else {
        size = m_buffer_index.Size() / sizeof(GLuint);
    }
    assert(size > 0);
    switch (m_layout) {
    case GL_TRIANGLES: assert(size % 3 == 0); break;
    case GL_TRIANGLE_STRIP: assert(size > 2); break;
    case GL_TRIANGLE_FAN: assert(size > 2 && (size & 1)); break;
    }
}
