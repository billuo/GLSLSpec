/**
 * @File Transform.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Math/Transform.hpp>


using namespace glm;

namespace Math {

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

} // namespace Math
