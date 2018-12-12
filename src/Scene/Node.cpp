/**
 * @File Node.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Scene/Node.hpp>
#include <Utility/Debug.hpp>


namespace Scene {

std::pair<Math::Degree, Math::Degree>
Node::get_orbit(const glm::vec3& center) const
{
    auto&& offset = m_transform.position - center;
    auto l_xz = length(glm::vec2(offset.x, offset.z));
    if (l_xz == 0.0f) {
        return {offset.y < 0.0f ? 90_deg : -90_deg, 0_deg};
    }
    if (offset.z < 0.0f) {
        return {Degree::atan(offset.y / l_xz), 180_deg - Degree::asin(offset.x / l_xz)};
    }
    return {Degree::atan(offset.y / l_xz), Degree::asin(offset.x / l_xz)};
}

} // namespace Scene
