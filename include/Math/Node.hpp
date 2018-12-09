#pragma once

#include "Transform.hpp"
#include "Utility/Misc.hpp"


namespace Math {

class Node {
  public:
    Node() = default;
    virtual ~Node() = default;

    explicit Node(const Transform& transform) : m_transform(transform)
    {}

    explicit Node(const glm::mat4& transform) : m_transform(transform)
    {}

    const Transform& transform() const
    { return m_transform; }

    enum class Axis : uint8_t { X, Y, Z, };

    glm::mat3 axes() const
    { return glm::mat3_cast(m_transform.rotation); }

    glm::vec3 axis(Axis axis)
    { return glm::row(glm::mat3_cast(m_transform.rotation), underlying_cast(axis)); }

    void reset()
    { on_reset(); }

    void set_position(const glm::vec3& position)
    { on_position(position); }

    void set_rotation(const glm::quat& rotation)
    { on_rotation(rotation); }

    void set_scale(const glm::vec3& scale)
    { on_scale(scale); }

    void translate_by(const glm::vec3& offset)
    { on_position(m_transform.position + offset); }

    void rotate_by(const glm::quat& quat)
    { on_rotation(m_transform.rotation * quat); }

    void scale_by(const glm::vec3& scale)
    { on_scale(m_transform.scale * scale); }

    float distance_to(const glm::vec3& position) const
    { return glm::length(position - m_transform.position); }

    void track(float distance)
    { translate_by(distance * axis(Axis::X)); }

    void pedestal(float distance)
    { translate_by(distance * axis(Axis::Y)); }

    void dolly(float distance)
    { translate_by(distance * axis(Axis::Z)); }

    void tilt(Degree degree)
    { rotate_by(glm::angleAxis(static_cast<float>(degree.radians()), axis(Axis::X))); }

    void pan(Degree degree)
    { rotate_by(glm::angleAxis(static_cast<float>(degree.radians()), axis(Axis::Y))); }

    void roll(Degree degree)
    { rotate_by(glm::angleAxis(static_cast<float>(degree.radians()), axis(Axis::Z))); }

    /// @brief Get the latitude & longitude of the orbit to a center
    /// @param center Center of the orbit
    /// @return a pair of Degree representing latitude and longitude resp.
    std::pair<Degree, Degree> get_orbit(const glm::vec3& center = glm::vec3(0.0f)) const;
  protected:
    Transform m_transform;

    // These callbacks are called BEFORE any changes occur to the underlying Transform.
    // The value after change is passed as an argument except in on_reset(),
    // where the result is assumed to be the identity Transform.
    // Currently, by default, they each set the result to the Transform, nothing else.
    // To actually assign the new value, either call these or assign it yourself in the overriding versions.

    virtual void on_reset()
    {
        set_position(glm::vec3(0.0f));
        set_rotation(glm::identity<glm::quat>());
        set_scale(glm::vec3(1.0f));
    }

    virtual void on_position(const glm::vec3& result)
    { m_transform.position = result; }

    virtual void on_rotation(const glm::quat& result)
    { m_transform.rotation = result; }

    virtual void on_scale(const glm::vec3& result)
    { m_transform.scale = result; }
};

} // namespace Math


