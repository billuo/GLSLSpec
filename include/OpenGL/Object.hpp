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

protected:
    Object() : m_name(0) { ; }
    ~Object() { m_name = 0; }

    GLuint m_name;

    /// Compains when Object is not initialized.
    bool aux_CheckInitialized(bool expected) const {
        if (Initialized() != expected) {
            DEBUG("OpenGL obejct not initialized before use.\n");
        }
        return true;
    }

private:
    Object(const Object&);            // disable
    Object& operator=(const Object&); // disable
};

} // namespace OpenGL

#endif /* end of include guard: OBJECT_HPP_NPLQXK1F */
