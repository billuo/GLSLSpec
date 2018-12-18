/**
 * @File Mesh.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Math/Math.hpp>
#include <OpenGL/VertexLayout.hpp>
#include <OpenGL/VertexBuffer.hpp>
#include <OpenGL/Introspection/Introspector.hpp>


class MeshBase {

  public:
    void draw(GLuint program)
    {
        update_layout(program);
        m_layout.bind();
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_n_vertices));
    }

    virtual void upload_all() = 0;

    virtual void update_layout(GLuint program) = 0;

  protected:
    /// Cached name of shader program used in vertex stage to draw this mesh.
    /// @note When expired, m_layout should be updated.
    GLuint m_vertex_program = 0;
    /// Contains all the vertex attributes this mesh provides.
    /// @details Any shader demanding less than what we have will work happily,
    /// but those expecting more should complain.
    OpenGL::VertexLayout m_layout;
    /// Cached number of vertices, available after data are all uploaded.
    size_t m_n_vertices;
};

// TODO maybe in the future use std::tuple for
//  - better generality
//  - clearer mapping between attributes and binding points (made the same as tuple index)
template <typename P, typename N, typename T, typename C = glm::vec3>
class Mesh : public MeshBase {
    template <typename _T>
    using VertexBuffer = OpenGL::VertexBuffer<_T>;
  public:
    Mesh(Owned<VertexBuffer<P>> positions, Owned<VertexBuffer<N>> normals,
         Owned<VertexBuffer<T>> tex_coords, Owned<VertexBuffer<C>> colors = {}) :
            m_positions(std::move(positions)),
            m_normals(std::move(normals)),
            m_tex_coords(std::move(tex_coords)),
            m_colors(std::move(colors))
    {}

    void upload_all() override
    {
        auto&& upload = [this](auto& vbo)
        {
            if (vbo) {
                this->m_n_vertices = vbo->size();
                vbo->upload();
            }
        };
        upload(m_positions);
        upload(m_normals);
        upload(m_tex_coords);
        upload(m_colors);
    }

    void update_layout(GLuint program) override
    {
        if (m_vertex_program == program) {
            return;
        }
        m_vertex_program = program;
        auto&& locked = OpenGL::Introspector::Get(m_vertex_program).lock();
        auto& input = locked->input();
        m_layout.clear();
        m_layout.bind();
        auto&& provide = [this, &input](auto& vbo)
        {
            if (vbo) {
                auto usage = vbo->usage();
                m_layout.define(OpenGL::VertexAttribute(usage));
                auto* a_input = input.find(m_layout.attribute(usage)->name);
                if (a_input) {
                    m_layout.bind_buffer(*vbo);
                    m_layout.attribute_name_me(a_input->location, usage);
                } else {
                    Log::w("{} not found", m_layout.attribute(usage)->name);
                }
            }
        };
        provide(m_positions);
        provide(m_normals);
        provide(m_tex_coords);
        provide(m_colors);
    }

  private:
    using MeshBase::m_n_vertices;
    using MeshBase::m_vertex_program;
    using MeshBase::m_layout;

    Owned<VertexBuffer<P>> m_positions;
    Owned<VertexBuffer<N>> m_normals;
    Owned<VertexBuffer<T>> m_tex_coords;
    Owned<VertexBuffer<C>> m_colors;
};

