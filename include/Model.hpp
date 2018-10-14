#ifndef MODEL_HPP_DW4MRMVP
#define MODEL_HPP_DW4MRMVP
#pragma once

#include "Mesh.hpp"

class Model {
public:
    Model() : m_scale(1.0f), m_orientation(glm::identity<glm::quat>()), m_pos(0.0f), m_valid(false) {}
    ~Model() {}

    Mesh& GetMesh() { return m_mesh; } /// @TODO encapsulate
    const Mesh& GetMesh() const { return m_mesh; }

    glm::mat4 GetTransform() const {
        if (!m_valid) {
            m_world_model = glm::translate(glm::mat4(1.0f), m_pos) * glm::scale(glm::mat4_cast(m_orientation), m_scale);
            m_valid = true;
        }
        return m_world_model;
    }

    void SetPos(glm::vec3 pos) {
        m_valid = false;
        m_pos = pos;
    }
    void SetScale(glm::vec3 scale) {
        m_valid = false;
        m_scale = scale;
    }
    void SetOrientation(glm::quat orientation) {
        m_valid = false;
        m_orientation = orientation;
    }

    glm::vec3 GetScale() const { return m_scale; }
    glm::vec3 GetPos() const { return m_pos; }
    glm::quat GetOrientation() const { return m_orientation; }

private:
    Model(const Model&);
    Model& operator=(const Model&);

    Mesh m_mesh;
    // XXX these are additional transformation upon model coordinates to get world coordinates.
    glm::vec3 m_scale;
    glm::quat m_orientation;
    glm::vec3 m_pos;
    mutable bool m_valid;
    mutable glm::mat4 m_world_model;
};

#endif /* end of include guard: MODEL_HPP_DW4MRMVP */
