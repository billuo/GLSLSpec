#include "Model.hpp"
#include "Debug.hpp"


Transform::Transform(glm::mat4 m) : Transform()
{
    if (glm::transpose(m)[3] == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
        position = glm::vec3(m[3][0], m[3][1], m[3][2]);
        // scale.x = Math::sign(glm::prod(m[0])) * glm::length(m[0]);
        // scale.y = Math::sign(glm::prod(m[1])) * glm::length(m[1]);
        // scale.z = Math::sign(glm::prod(m[2])) * glm::length(m[2]);
        scale.x = glm::length(m[0]);
        scale.y = glm::length(m[1]);
        scale.z = glm::length(m[2]);
        glm::mat3 r_m(m);
        r_m[0] /= scale.x;
        r_m[1] /= scale.y;
        r_m[2] /= scale.z;
        rotation = glm::quat(r_m); // r_m needs to be pure rotation matrix
    }
}

