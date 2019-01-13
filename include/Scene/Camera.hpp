/**
 * @File Camera.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Node.hpp"


namespace Scene {

class Camera : public Node {
    using Degree = Math::Degree;

  public:

    enum Projection { Perspective, Orthographic };

    Camera(glm::vec3 pos, glm::vec3 target);

    const glm::mat4& projection_world() const;
    const glm::mat4& projection_view() const;
    const glm::mat4& view_world() const;
    const glm::mat3& normal_matrix() const;

    glm::vec3 world_to_view(glm::vec3 vec) const;
    glm::vec3 world_to_projection(glm::vec3 vec) const;

    Projection projection = Perspective; // TODO encapsulate to invalidate matrices cache

    auto look_direction() const
    { return m_direction; }

    auto clip() const
    { return m_clip; }

    glm::vec3 view_axis(Axis axis);

    /// @note Positive distance moves the node rightward (towards <B>X</B>)
    void track(float distance)
    { translate_by(distance * view_axis(Axis::X)); }

    /// @note Positive distance moves the node upward (towards <B>Y</B>)
    void pedestal(float distance)
    { translate_by(distance * view_axis(Axis::Y)); }

    /// @note Positive distance moves the node forward (towards <B>-Z</B>)
    void dolly(float distance)
    { translate_by(-distance * view_axis(Axis::Z)); }

    void tilt(Degree degrees)
    { rotate_by(glm::angleAxis(degrees.radians().value(), view_axis(Axis::X))); }

    void pan(Degree degrees)
    { rotate_by(glm::angleAxis(degrees.radians().value(), view_axis(Axis::Y))); }

    void roll(Degree degrees)
    { rotate_by(glm::angleAxis(degrees.radians().value(), view_axis(Axis::Z))); }

    void set_aspect(float aspect)
    {
        m_aspect = aspect;
        m_matrices.cached = false;
    }

    /// @brief Update look direction to look at specified target.
    /// @param target The target point to look at in world coord.
    void look_at(const glm::vec3& target)
    { set_look(Math::LngLat{target, get_position(), axis(Axis::Y), axis(Axis::X)}); }

    void set_look(Math::LngLat lnglat)
    {
        lnglat.latitude.round_half();
        lnglat.longitude.clamp(89.9f);
        aux_set_look(lnglat);
    }

    /// @brief Set latitude of look direction
    void look_lat(Degree degrees)
    {
        degrees.round_half();
        aux_set_look({m_direction.longitude, degrees});
    }

    /// @brief Set longitude of look direction
    void look_lon(Degree degrees)
    {
        degrees.clamp(89.9f);
        aux_set_look({degrees, m_direction.latitude});
    }

    /// @brief Change look direction horizontally
    /// @note It's addition rather than assignment
    void horizontal_look(Degree degrees)
    { look_lat(m_direction.latitude + degrees); }

    /// @brief Change look direction vertically
    /// @note It's addition rather than assignment
    void vertical_look(Degree degrees)
    { look_lon(m_direction.longitude + degrees); }

  protected:

    void on_position(const glm::vec3& result) override
    { m_matrices.cached = false; }

    void on_rotation(const glm::quat& result) override
    { m_matrices.cached = false; }

  private:
    struct Matrices {
        /// True if all the matrices computed last time are still valid
        bool cached{false};
        /// view -> NDC
        glm::mat4 projection_world{1.0f};
        /// world -> view
        glm::mat4 view_world{1.0f};
        /// world -> NDC
        glm::mat4 projection_view{1.0f};
        /// transpose inverse of view_world to cancel out non-uniform scaling
        glm::mat3 normal{1.0f};
    };
    mutable Matrices m_matrices{};

    /// Try to update all cached matrices
    void compute_all() const;

    void aux_set_look(Math::LngLat lnglat)
    {
        m_direction = lnglat;
        m_matrices.cached = false;
    }

    /// LngLat of the target point we are looking at on the unit sphere centered at this camera.
    Math::LngLat m_direction{0_deg, 90_deg};
    /// Distance of the near/far clipping planes.
    glm::vec2 m_clip{1.0f / 128.0f, 128.0f};
    /// Field Of View (vertical; y direction)
    Degree m_FOV{45_deg};
    /// Aspect ratio (width/height)
    float m_aspect{1.0f};
};

} // namespace Math

