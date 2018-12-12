/**
 * @File Model.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Scene/Node.hpp>
#include <Mesh.hpp>


namespace Scene {

class Model : public Node {
  public:
    Model() = default;
    ~Model() = default;
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    Math::Transform transform;
    Mesh mesh;
};

} //namespace Scene
