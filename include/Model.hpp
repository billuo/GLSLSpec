#ifndef MODEL_HPP_DW4MRMVP
#define MODEL_HPP_DW4MRMVP
#pragma once

#include "Mesh.hpp"


class Transform {
  public:
    Transform()
            : scale(1.0f), rotation(glm::identity<glm::quat>()), position(0.0f)
    {}

    explicit Transform(glm::mat4 matrix) : Transform()
    {
        if (glm::transpose(matrix)[3] == glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
            // OPT rid of transpose
            position = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
            scale.x = Math::sign(glm::prod(matrix[0])) * glm::length(matrix[0]);
            scale.y = Math::sign(glm::prod(matrix[1])) * glm::length(matrix[1]);
            scale.z = Math::sign(glm::prod(matrix[2])) * glm::length(matrix[2]);
            rotation = glm::quat(glm::mat3(matrix));
        }
    }

    explicit operator glm::mat4()
    {
        return glm::translate(glm::mat4(1.0f), position) *
               glm::scale(glm::mat4_cast(rotation), scale);
    }

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 position;
};

class Model {
  public:
    Model() = default;
    ~Model() = default;
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    Transform transform;
    Mesh mesh;
};

#endif /* end of include guard: MODEL_HPP_DW4MRMVP */
