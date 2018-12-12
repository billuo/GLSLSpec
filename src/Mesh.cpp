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
          m_colors(std::move(colors))
{}

void
Mesh::draw()
{
    m_layout.bind();
}

void
Mesh::upload()
{
}
