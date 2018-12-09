#include "Math/Node.hpp"
#include "Math/Camera.hpp"
#include "Utility/Debug.hpp"


namespace Math {

Camera::Camera(glm::vec3 pos, glm::vec3 target) : Node()
{
    set_position(pos);
    look_at(target);
}

glm::mat4
Camera::projection_world() const
{
    compute_all();
    return m_matrices.projection_world;
}

glm::mat4
Camera::projection_view() const
{
    compute_all();
    return m_matrices.projection_view;
}

glm::vec3
Camera::world_to_view(glm::vec3 vec) const
{
    compute_all();
    return m_matrices.view_world * glm::vec4(vec, 0.0f);
}

glm::vec3
Camera::world_to_projection(glm::vec3 vec) const
{
    compute_all();
    return m_matrices.projection_world * glm::vec4(vec, 0.0f);
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
    m_matrices.view_world = glm::lookAt(Node::m_transform.position, m_look_at, m_up);
    m_matrices.projection_world = m_matrices.projection_view * m_matrices.view_world;
    m_matrices.cached = true;
}

void
Camera::set_orbit(Degree lat, Degree lon, float radius, const glm::vec3& center, const glm::vec3& up)
{
    lat.clamp(89.9f);
    auto lat_quat = angleAxis(static_cast<float>(lat.radians()), glm::vec3(-1.0f, 0.0f, 0.0f));
    auto lon_quat = angleAxis(static_cast<float>(lon.radians()), glm::vec3(0.0f, 1.0f, 0.0f));
    set_position(lon_quat * (lat_quat * glm::vec3(0.0f, 0.0f, radius)) + center);
    look_at(center, up);
}

void
Camera::orbit(Degree dlat, Degree dlon, float dr, const glm::vec3& center, const glm::vec3& up)
{
    auto&&[lat, lon] = get_orbit(center);
    set_orbit(lat + dlat, lon + dlon, distance_to(center) + dr, center, up);
}

} // namespace Math
