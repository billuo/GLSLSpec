/**
 * @File Transform.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Math/Transform.hpp>


using namespace glm;

namespace Math {

LngLat::LngLat(const glm::vec3& p, const glm::vec3& center, const glm::vec3& up, const glm::vec3& right)
{
    const auto front = glm::cross(up, right);
    const auto d = glm::normalize(p - center);
    const auto c = glm::dot(d, up);
    const auto proj = d - c * up;
    const auto y = glm::dot(proj, front);
    const auto x = glm::dot(proj, right);
    longitude = Degree::asin(c);
    latitude = Degree::atan(y, x);
}

glm::vec3
LngLat::position(const glm::vec3& center, float radius) const
{
    auto s = longitude.sin();
    auto c = longitude.cos();
    return center + radius * glm::vec3(c * latitude.cos(), s, -c * latitude.sin());
}

std::ostream&
operator<<(std::ostream& os, LngLat rhs)
{
    return os << '{' << rhs.longitude << ", " << rhs.latitude << '}';
}

Transform::Transform(glm::mat4 m) : Transform()
{
    using namespace glm;
    if (transpose(m)[3] == vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
        position = vec3(m[3][0], m[3][1], m[3][2]);
        scale.x = length(m[0]);
        scale.y = length(m[1]);
        scale.z = length(m[2]);
        scale *= Math::sign(m[0][0] * m[1][1] * m[2][2] * m[3][3]);
        mat3 r_m(m);
        r_m[0] /= scale.x;
        r_m[1] /= scale.y;
        r_m[2] /= scale.z;
        rotation = quat(r_m); // r_m needs to be pure rotation matrix
    }
}

glm::mat3
Transform::align(const glm::vec3& axis, const glm::vec3& reference)
{
    const auto v = glm::cross(axis, reference);
    const auto cos = glm::dot(axis, reference);
    const auto k = 1.0f / (1.0f + cos);
    return glm::mat3(v.x * v.x * k + cos, v.x * v.y * k + v.z, v.x * v.z * k - v.y,
                     v.y * v.x * k - v.z, v.y * v.y * k + cos, v.y * v.z * k + v.x,
                     v.z * v.x * k + v.y, v.z * v.y * k - v.x, v.z * v.z * k + cos);
}

glm::mat4
Transform::to_mat4() const
{ return glm::translate(mat4(1.0f), position) * glm::scale(glm::mat4_cast(rotation), scale); }

} // namespace Math
