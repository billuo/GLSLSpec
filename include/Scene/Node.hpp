/**
 * @File Node.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Math/Transform.hpp>
#include <Utility/Misc.hpp>
#include <Utility/Debug.hpp>


namespace Scene {

class Node {
    using Transform = Math::Transform;
    using Degree = Math::Degree;
    using Radian = Math::Radian;

  public:
    Node() = default;
    virtual ~Node() = default;

    explicit Node(const Transform& transform) : m_transform(transform)
    {}

    explicit Node(const glm::mat4& transform_matrix) : m_transform(transform_matrix)
    {}

    //region trivial getter/setters

    const Transform& transform() const
    { return m_transform; }

    auto get_position() const
    { return m_transform.position; }

    auto get_rotation() const
    { return m_transform.rotation; }

    auto get_scale() const
    { return m_transform.scale; }

    /// @brief Obtain all basis vectors in order as column vectors.
    glm::mat3 axes() const
    { return glm::mat3_cast(m_transform.rotation); }

    enum class Axis { X, Y, Z, };

    /// @brief Obtain a specified basis vector.
    glm::vec3 axis(Axis axis) const
    { return glm::mat3_cast(m_transform.rotation)[underlying_cast(axis)]; }

    void reset()
    {
        on_reset();
        m_transform = Math::Transform();
    }

    void set_position(const glm::vec3& position)
    {
        on_position(position);
        m_transform.position = position;
    }

    void set_rotation(const glm::quat& rotation)
    {
        on_rotation(rotation);
        m_transform.rotation = rotation;
    }

    void set_scale(const glm::vec3& scale)
    {
        on_scale(scale);
        m_transform.scale = scale;
    }

    void translate_by(const glm::vec3& offset)
    { set_position(offset + m_transform.position); }

    /// @note It applies the rotation locally; i.e. multiplied on the left.
    void rotate_by(const glm::quat& quat)
    { set_rotation(quat * m_transform.rotation); }

    void scale_by(const glm::vec3& scale)
    { set_scale(scale * m_transform.scale); }


    //endregion

    // TODO cache a rotation mat3 maybe?
    //      - and if we do, on_rotation needs to be redesigned and maybe we should insist on
    //      - subclasses calling the base versions since they don't (and shouldn't) know
    //      - how to invalidate or update the cached mat3.

    /// @brief Return the rotation matrix that aligns the specified axis with a <B>normalized</B> vector.
    /// @note Result is undefined when the reference vector points to the opposite direction of the specified axis.
    glm::mat3 align(Axis axis, const glm::vec3& reference) const
    { return Transform::align(this->axis(axis), reference); }

    /// @brief Align one of the axis of this node with a <B>normalized</B> vector.
    /// @sa align()
    void align_with(Axis axis, const glm::vec3& reference)
    { rotate_by(glm::quat_cast(align(axis, reference))); }

    float distance(const glm::vec3& position) const
    { return glm::length(position - get_position()); }

    void distance(const glm::vec3& position, float distance)
    {
        auto&& dir = glm::normalize(get_position() - position);
        set_position(position + distance * dir);
    }

  protected:
    // These callbacks are called BEFORE any changes occur to the underlying Transform.
    // The new value to change to is passed as an argument, except for on_reset();
    // where the result is assumed to be the identity Transform.
    // By default, they do nothing. The result value is also automatically assigned.
    // To actually assign the new value, either call these base versions or do the assignment in subclasses.

    virtual void on_reset()
    {}

    virtual void on_position(const glm::vec3& result)
    {}

    virtual void on_rotation(const glm::quat& result)
    {}

    virtual void on_scale(const glm::vec3& result)
    {}

  private:
    Transform m_transform;

};

} // namespace Scene


