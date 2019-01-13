/**
 * @File Transform.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Math.hpp"


namespace Math {

struct LngLat {
    LngLat() = default;

    LngLat(Degree lng, Degree lat) : longitude(lng), latitude(lat)
    {}

    /// @brief Compute the latitude & longitude of a point relative to the given references
    /// @details Facing 'front', the left side has positive latitude.
    /// @param p The relative point.
    /// @param center The center point as a reference.
    /// @param up Normalized vector on whose direction longitude=90.
    /// @param right Normalized vector on whose direction latitude=0.
    /// @return The pair of longitude & latitude in degrees.
    LngLat(const glm::vec3& p, const glm::vec3& center, const glm::vec3& up, const glm::vec3& right);

    glm::vec3 position(const glm::vec3& center = glm::vec3(0.0f), float radius = 1.0f) const;

    Degree longitude{0_deg};
    Degree latitude{0_deg};

    friend std::ostream& operator<<(std::ostream& os, LngLat rhs);
};

class Transform {
    using vec3 = glm::vec3;
    using quat = glm::quat;
    using mat4 = glm::mat4;
  public:
    Transform() = default;

    Transform(vec3 scale, quat rotation, vec3 position) : scale(scale), rotation(rotation), position(position)
    {}

    explicit Transform(mat4 matrix);

    mat4 to_mat4() const;

    vec3 scale{1.0f};
    quat rotation{glm::identity<quat>()};
    vec3 position{0.0f};

    /// @brief Return the rotation matrix that aligns the specified axis with a <B>normalized</B> vector.
    /// @param axis The axis to align.
    /// @param reference The axis to align to.
    /// @return Rotation matrix R s.t. R * @p axis == @p reference.
    /// @note Result is undefined when the reference vector points to the opposite direction of the specified axis.
    static glm::mat3 align(const glm::vec3& axis, const glm::vec3& reference);
};

} // namespace Math
