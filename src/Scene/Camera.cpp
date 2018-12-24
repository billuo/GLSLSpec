/**
 * @File Camera.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Scene/Camera.hpp>


namespace Scene {

using Degree = Math::Degree;

Camera::Camera(glm::vec3 pos, glm::vec3 target) : Node()
{
    set_position(pos);
    look_at(target);
}

const glm::mat4&
Camera::projection_world() const
{
    compute_all();
    return m_matrices.projection_world;
}

const glm::mat4&
Camera::projection_view() const
{
    compute_all();
    return m_matrices.projection_view;
}

const glm::mat4&
Camera::view_world() const
{
    compute_all();
    return m_matrices.view_world;
}

const glm::mat3&
Camera::normal_matrix() const
{
    compute_all();
    return m_matrices.normal;
}

glm::vec3
Camera::world_to_view(glm::vec3 vec) const
{
    compute_all();
    return m_matrices.view_world * glm::vec4(vec, 1.0f);
}

glm::vec3
Camera::world_to_projection(glm::vec3 vec) const
{
    compute_all();
    return m_matrices.projection_world * glm::vec4(vec, 1.0f);
}

void
Camera::compute_all() const
{
    if (m_matrices.cached) {
        return;
    }
    if (projection == Perspective) {
        m_matrices.projection_view = glm::perspective(m_FOV.radians().value(), m_aspect, m_clip.x, m_clip.y);
    } else {
        m_matrices.projection_view = glm::ortho(-3 * m_aspect, 3 * m_aspect, -3.0f, 3.0f, -10.0f, 10.0f);
    }
    m_matrices.view_world =
            glm::lookAt(get_position(), get_position() + get_rotation() * m_direction.position(), axis(Axis::Y));
    m_matrices.projection_world = m_matrices.projection_view * m_matrices.view_world;
    m_matrices.normal = glm::transpose(glm::inverse(glm::mat3(m_matrices.view_world)));
    m_matrices.cached = true;
}

glm::vec3
Camera::view_axis(Node::Axis axis)
{
    switch (axis) {
        case Axis::X:
            return glm::normalize(glm::cross(m_direction.position(), this->axis(Axis::Y)));
        case Axis::Y:
            return this->axis(Axis::Y);
        case Axis::Z:
            return -m_direction.position();
        default:
            UNREACHABLE;
    }
}

} // namespace Scene
