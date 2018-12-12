/**
 * @File Mesh.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Math/Math.hpp>
#include <OpenGL/VertexLayout.hpp>
#include <OpenGL/Introspection/Introspector.hpp>


class Mesh {
    using Position = glm::vec3;
    using Normal = glm::vec3;
    using TexCoord = glm::vec2;
    using Color = glm::vec3;
  public:
    Mesh(OpenGL::VertexLayout layout, std::vector<Position> positions, std::vector<Normal> normals,
         std::vector<TexCoord> tex_coords, std::vector<Color> colors = {});

    ~Mesh() = default;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void draw(Shared<OpenGL::Introspector> current);

    void upload();

  private:
    OpenGL::VertexLayout m_layout;
    Shared<OpenGL::Introspector> m_vertex_stage;
    std::vector<Position> m_positions;
    std::vector<Normal> m_normals;
    std::vector<TexCoord> m_tex_coords;
    std::vector<Color> m_colors;
    size_t m_n_vertices;

    struct Buffers {
        using Buffer = OpenGL::Buffer;
        Buffer position;
        Buffer normal;
        Buffer tex_coord;
        Buffer color;

        const Buffer& buffer_for(OpenGL::VertexAttribute::Usage usage) const;
    };

    Owned<Buffers> m_buffers;

    void aux_update_layout_indices();
};

