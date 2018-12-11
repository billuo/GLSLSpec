#ifndef OPENGL_LAB_TRANSFORM_HPP
#define OPENGL_LAB_TRANSFORM_HPP
#pragma once

#include "Math.hpp"


namespace Math {

class Transform {
    using vec3 = glm::vec3;
    using quat = glm::quat;
    using mat4 = glm::mat4;
  public:
    Transform() = default;

    Transform(vec3 scale, quat rotation, vec3 position) : scale(scale), rotation(rotation), position(position)
    {}

    explicit Transform(mat4 matrix);

    mat4 to_mat4() const
    {
        return glm::translate(mat4(1.0f), position) * glm::scale(glm::mat4_cast(rotation), scale);
    }

    vec3 scale{1.0f};
    quat rotation{glm::identity<quat>()};
    vec3 position{0.0f};
};

} // namespace Math



#endif //OPENGL_LAB_TRANSFORM_HPP
