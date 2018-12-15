/**
 * @File Camera.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Scene/Camera.hpp>


namespace Scene {
using Degree = Math::Degree;

Camera::Camera(glm::vec3 pos, glm::vec3 target, const glm::vec3& up) : Node(), m_up(up)
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
        m_matrices.projection_view = glm::perspective(m_FOV, m_aspect, m_clip.x, m_clip.y);
    } else {
        m_matrices.projection_view = glm::ortho(-3 * m_aspect, 3 * m_aspect, -3.0f, 3.0f, -10.0f, 10.0f);
    }
    m_matrices.view_world = glm::lookAt(m_transform.position, m_transform.position + look_dir(), m_up);
    m_matrices.projection_world = m_matrices.projection_view * m_matrices.view_world;
    m_matrices.normal = glm::transpose(glm::inverse(glm::mat3(m_matrices.view_world)));
    m_matrices.cached = true;
}

void
Camera::set_orbit(Degree lat, Degree lon, const glm::vec3& center, const glm::vec3& up)
{
    lat.clamp(89.9f);
    m_angle.vertical = -lat;
    m_angle.horizontal = -lon;
    m_angle.horizontal.round_half();
    auto lat_quat = angleAxis(static_cast<float>(lat.radians()), glm::vec3(-1.0f, 0.0f, 0.0f));
    auto lon_quat = angleAxis(static_cast<float>(lon.radians()), glm::vec3(0.0f, 1.0f, 0.0f));
    set_position(lon_quat * (lat_quat * glm::vec3(0.0f, 0.0f, distance_to(center))) + center);
    look_at(center, up);
}

void
Camera::orbit(Degree dlat, Degree dlon, const glm::vec3& center, const glm::vec3& up)
{
    auto&&[lat, lon] = get_orbit(center);
    set_orbit(lat + dlat, lon + dlon, center, up);
}

} // namespace Scene
