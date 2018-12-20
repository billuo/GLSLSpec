/**
 * @File Camera.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Scene/Node.hpp>


namespace Scene {

class Camera : public Node {
    using Degree = Math::Degree;

  public:

    enum Projection { Perspective, Orthographic };

    Camera(glm::vec3 pos, glm::vec3 target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    const glm::mat4& projection_world() const;
    const glm::mat4& projection_view() const;
    const glm::mat4& view_world() const;
    const glm::mat3& normal_matrix() const;

    glm::vec3 world_to_view(glm::vec3 vec) const;
    glm::vec3 world_to_projection(glm::vec3 vec) const;

    Projection projection = Perspective;

    using Node::distance_to;

    void set_aspect(float aspect)
    {
        m_aspect = aspect;
        m_matrices.cached = false;
    }

    using Node::look_at;

    void look_at(const glm::vec3& target)
    {
        look_at(target, m_up);
        auto&&[lat, lon] = get_orbit(target);
        m_angle.vertical = -lat;
        m_angle.horizontal = -lon;
    }

    /// @brief Move the camera on the orbit around current look_at.
    /// @param dlat Offset of latitude
    /// @param dlon Offset of longitude
    /// @param center Center of the orbit
    void orbit(Degree dlat, Degree dlon, const glm::vec3& center)
    { orbit(dlat, dlon, center, m_up); }

    /// @brief Set the node onto the orbit around @p center.
    /// @param lat Latitude of the orbit
    /// @param lon Longitude of the orbit
    /// @param center Center of the orbit
    /// @param up Reference direction of up in world coord.
    void set_orbit(Degree lat, Degree lon, const glm::vec3& center, const glm::vec3& up);

    /// @brief Change orbit relatively
    /// @param dlat Offset of latitude
    /// @param dlon Offset of longitude
    /// @param center Center of the orbit, defaults to world origin.
    /// @param up Reference direction of up in world coord.
    void orbit(Degree dlat, Degree dlon, const glm::vec3& center, const glm::vec3& up);

    void tilt(Degree degree)
    {
        m_angle.vertical += degree;
        m_angle.vertical.clamp(89.9f);
        m_matrices.cached = false;
    }

    void pan(Degree degree)
    {
        m_angle.horizontal += degree;
        m_angle.horizontal.round_half();
        m_matrices.cached = false;
    }

    void roll(Degree degree)
    {
        // TODO
    }

    auto look_dir() const
    {
        auto& h = m_angle.horizontal;
        auto& v = m_angle.vertical;
        return glm::vec3(v.cos() * h.sin(), v.sin(), v.cos() * -h.cos());
    }

    auto clip() const
    { return m_clip; }

  protected:

    void on_position(const glm::vec3& result) override
    {
        Node::on_position(result);
        m_matrices.cached = false;
    }

    void on_rotation(const glm::quat& result) override
    {
        Node::on_rotation(result);
        // adjust m_angle ...?
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
        /// transpose inverse of view_world to cancel out non-uniform scaling
        glm::mat3 normal = glm::mat3(1.0f);
    };
    mutable Matrices m_matrices;

    /// View angle to calculate direction looking at
    struct {
        Degree horizontal = 0_deg;
        Degree vertical = 0_deg;
    } m_angle;

    /// Update all cached matrices
    void compute_all() const;

    glm::vec3 m_up;
    glm::vec2 m_clip = glm::vec2(1.0f / 128.0f, 128.0f);
    float m_FOV = 45.0f;
    float m_aspect = 1.0f;
};

} // namespace Math

