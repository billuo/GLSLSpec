#ifndef MODEL_HPP_DW4MRMVP
#define MODEL_HPP_DW4MRMVP
#pragma once

#include "Math/Transform.hpp"
#include "Mesh.hpp"


class Model {
  public:
    Model() = default;
    ~Model() = default;
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    Math::Transform transform;
    Mesh mesh;
};

#endif /* end of include guard: MODEL_HPP_DW4MRMVP */
