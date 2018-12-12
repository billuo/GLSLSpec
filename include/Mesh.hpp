/**
 * @File Mesh.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Math/Math.hpp>
#include <OpenGL/VertexLayout.hpp>


class Mesh {
  public:
    Mesh(OpenGL::VertexLayout layout, std::vector<glm::vec3> positions, std::vector<glm::vec3> normals,
         std::vector<glm::vec2> tex_coords, std::vector<glm::vec3> colors = {});

    ~Mesh() = default;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    void draw();

    void upload();

  private:
    OpenGL::VertexLayout m_layout;
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_tex_coords;
    std::vector<glm::vec3> m_colors;
    struct Buffers {
        using Buffer = OpenGL::Buffer;
        Buffer position;
        Buffer normal;
        Buffer tex_coord;
        Buffer color;
    };
    Owned<Buffers> m_buffers;
};

