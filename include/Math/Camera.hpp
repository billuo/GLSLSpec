#pragma once

#include "Node.hpp"
#include "Transform.hpp"


namespace Math {

class Camera : public Node {
  public:
    enum Projection { Perspective, Orthographic };

    explicit Camera(glm::vec3 pos, glm::vec3 target = glm::vec3(0.0f));

    glm::mat4 projection_world() const;
    glm::mat4 projection_view() const;

    glm::vec3 world_to_view(glm::vec3 vec) const;
    glm::vec3 world_to_projection(glm::vec3 vec) const;

    Projection projection = Perspective;

    using Node::distance_to;

    float distance_to() const
    { return distance_to(m_look_at); }

    void set_aspect(float aspect)
    {
        m_aspect = aspect;
        m_matrices.cached = false;
    }

    auto look_at() const
    { return m_look_at; }

    void look_at(const glm::vec3& target)
    { look_at(target, m_up); }

    void look_at(const glm::vec3& target, const glm::vec3& up)
    {
        m_look_at = target;
        m_up = up;
        set_rotation(quat_cast(lookAt(-m_transform.position, target, up)));
    }

    /// @brief Move the camera on the orbit around current look_at.
    /// @param dlat Offset of latitude
    /// @param dlon Offset of longitude
    /// @param dr Offset of the distance to look_at, defaults to 0.0f.
    void orbit(Degree dlat, Degree dlon, float dr = 0.0f)
    {
        orbit(dlat, dlon, dr, look_at(), m_up);
        // XXX DON'T replace look_at() with m_look_at.
        // In set_orbit(), m_look_at is set to the orbit center at last,
        // which is passed by reference all the way.
        // However just before that, the position is set and as a result,
        // m_look_at is updated (falsely) as well. If m_look_at itself is passed in,
        // The correction to m_look_at is invalid -- it's just self assignment.
    }

    /// @brief Set the node onto the orbit around a center.
    /// @param lat Latitude of the orbit
    /// @param lon Longitude of the orbit
    /// @param radius Distance to the orbit center
    /// @param center Center of the orbit, defaults to world origin.
    /// @param up Reference direction of up in world coord.
    void set_orbit(Degree lat, Degree lon, float radius, const glm::vec3& center, const glm::vec3& up);
    /// @brief Change orbit relatively
    /// @param dlat Offset of latitude
    /// @param dlon Offset of longitude
    /// @param dr Offset of the distance to the orbit center
    /// @param center Center of the orbit, defaults to world origin.
    /// @param up Reference direction of up in world coord.
    void orbit(Degree dlat, Degree dlon, float dr, const glm::vec3& center, const glm::vec3& up);
  protected:

    void on_position(const glm::vec3& result) override
    {
        m_look_at += result - m_transform.position;
        Node::on_position(result);
        m_matrices.cached = false;
    }

    void on_rotation(const glm::quat& result) override
    {
        Node::on_rotation(result);
        m_matrices.cached = false;
    }

  private:
    struct Matrices {
        /// True if all the matrices computed last time are still valid
        bool cached = false;
        /// view -> NDC
        glm::mat4 projection_world = glm::mat4(1.0f);
        /// world -> view
        glm::mat4 view_world = glm::mat4(1.0f);
        /// world -> NDC
        glm::mat4 projection_view = glm::mat4(1.0f);
    };
    mutable Matrices m_matrices;

    /// Update all cached matrices
    void compute_all() const;

    glm::vec3 m_look_at = glm::vec3(0.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec2 m_clip = glm::vec2(1.0f / 128.0f, 128.0f);
    float m_FOV = 45.0f;
    float m_aspect = 4.0f / 3.0f;
};

} // namespace Math

