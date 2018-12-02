#ifndef OPENGL_LAB_TRANSFORM_HPP
#define OPENGL_LAB_TRANSFORM_HPP
#pragma once

#include "Math.hpp"


class Transform {
    using vec3 = glm::vec3;
    using quat = glm::quat;
    using mat4 = glm::mat4;
  public:
    Transform() : scale(1.0f), rotation(glm::identity<quat>()), position(0.0f)
    {}

    explicit Transform(mat4 matrix);

    Transform(vec3 scale, quat rotation, vec3 position)
            : scale(scale), rotation(rotation), position(position)
    {}

    mat4 to_mat4() const
    {
        return glm::translate(mat4(1.0f), position) *
               glm::scale(glm::mat4_cast(rotation), scale);
    }

    explicit operator mat4() const
    {
        return to_mat4();
    }

    vec3 scale;
    quat rotation;
    vec3 position;
};

#endif //OPENGL_LAB_TRANSFORM_HPP
