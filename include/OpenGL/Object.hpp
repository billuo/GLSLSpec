/**
 * @file OpenGL/Object.hpp
 * @brief Base class representing generic OpenGL objects, each contains a name of type GLuint of OpenGL object.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef OBJECT_HPP_NPLQXK1F
#define OBJECT_HPP_NPLQXK1F
#pragma once

#include "Debug.hpp"
#include "OpenGL/Common.hpp"
#include "Utility.hpp"

namespace OpenGL {

/**
 * @class Object
 * @brief Base class of OpenGL objects.
 */
template <typename Derived>
class Object {
public:
    bool Initialized() const { return m_name != 0; }
    GLuint Name() const { return m_name; }

public:
    Object() : m_name(0) {}
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&& obj) : m_name(obj.m_name) { obj.m_name = 0; }
    Object& operator=(Object&& rhs) {
        GLuint name = rhs.m_name;
        rhs.m_name = 0;
        m_name = name;
    }

    virtual ~Object() = 0;

protected:
    void SetName(GLuint name) { m_name = name; }
    /// Compains when Object is not initialized as expected.
    bool aux_CheckInitialized(bool expected) const {
        if (Initialized() != expected) {
            DEBUG("OpenGL obejct<type=%s> not initialized before use.\n", type_name<Derived>());
        }
        return true;
    }

private:
    GLuint m_name;
};

template <typename Derived>
Object<Derived>::~Object() {
    m_name = 0;
}

} // namespace OpenGL

#endif /* end of include guard: OBJECT_HPP_NPLQXK1F */
