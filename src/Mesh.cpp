/**
 * @File Mesh.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Mesh.hpp>
#include <Utility/Debug.hpp>


Mesh::Mesh(OpenGL::VertexLayout layout, std::vector<glm::vec3> positions, std::vector<glm::vec3> normals,
           std::vector<glm::vec2> tex_coords, std::vector<glm::vec3> colors)
        : m_layout(std::move(layout)),
          m_positions(std::move(positions)),
          m_normals(std::move(normals)),
          m_tex_coords(std::move(tex_coords)),
          m_colors(std::move(colors)),
          m_n_vertices(m_positions.size())
{
    assert(m_normals.empty() || m_normals.size() == m_n_vertices);
    assert(m_tex_coords.empty() || m_tex_coords.size() == m_n_vertices);
    assert(m_colors.empty() || m_colors.size() == m_n_vertices);
}

void
Mesh::draw(Shared<OpenGL::Introspector> current)
{
    if (!m_buffers) {
        upload();
    }
    if (current != m_vertex_stage) {
        m_vertex_stage = current;
        aux_update_layout_indices();
    }
    m_layout.bind();
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_n_vertices);
}

void
Mesh::upload()
{
    if (m_buffers) {
        WARNING("Already uploaded");
    }
    m_buffers = std::make_unique<Buffers>();
    using Usage = OpenGL::VertexAttribute::Usage;
    using Buffer = OpenGL::Buffer;
    if (m_layout.attribute(Usage::Position)) {
        assert(!m_positions.empty());
        Buffer::Bind(GL_ARRAY_BUFFER, m_buffers->position);
        Buffer::Data(GL_ARRAY_BUFFER, m_positions.size() * sizeof(Position), m_positions.data(), GL_STATIC_DRAW);
        m_positions.clear();
    } else {
        Log::e("Mesh with no vertex positions...?");
    }
    if (m_layout.attribute(Usage::Normal)) {
        assert(!m_normals.empty());
        Buffer::Bind(GL_ARRAY_BUFFER, m_buffers->normal);
        Buffer::Data(GL_ARRAY_BUFFER, m_normals.size() * sizeof(Normal), m_normals.data(), GL_STATIC_DRAW);
        m_normals.clear();
    }
    if (m_layout.attribute(Usage::TexCoord)) {
        assert(!m_tex_coords.empty());
        Buffer::Bind(GL_ARRAY_BUFFER, m_buffers->tex_coord);
        Buffer::Data(GL_ARRAY_BUFFER, m_tex_coords.size() * sizeof(TexCoord), m_tex_coords.data(), GL_STATIC_DRAW);
        m_tex_coords.clear();
    }
    if (m_layout.attribute(Usage::Color)) {
        assert(!m_colors.empty());
        Buffer::Bind(GL_ARRAY_BUFFER, m_buffers->color);
        Buffer::Data(GL_ARRAY_BUFFER, m_colors.size() * sizeof(Color), m_colors.data(), GL_STATIC_DRAW);
        m_colors.clear();
    }
}

void
Mesh::aux_update_layout_indices()
{
    for (auto&& attr : m_layout) {
        if (attr) {
            auto* input = m_vertex_stage->input().find(attr->name.c_str());
            if (input) {
                m_layout.bind_buffer(m_buffers->buffer_for(attr->usage), attr->usage);
                m_layout.attribute_binding(input->location, underlying_cast(attr->usage));
                m_layout.enable(input->location);
            } else {
                Log::w("Attribute {} with usage {} expected in shader.", attr->name, to_string(attr->usage));
            }
        }
    }
}

const Mesh::Buffers::Buffer&
Mesh::Buffers::buffer_for(OpenGL::VertexAttribute::Usage usage) const
{
    switch (usage) {
        case OpenGL::VertexAttribute::Usage::Position:
            return position;
        case OpenGL::VertexAttribute::Usage::Color:
            return color;
        case OpenGL::VertexAttribute::Usage::Normal:
            return normal;
        case OpenGL::VertexAttribute::Usage::TexCoord:
            return tex_coord;
        case OpenGL::VertexAttribute::Usage::Tangent:
            break;
        case OpenGL::VertexAttribute::Usage::Other:
            break;
        case OpenGL::VertexAttribute::Usage::Max:
            break;
    }
    throw unimplemented("buffer of usage " + to_string(usage) + " in meshes");
}
