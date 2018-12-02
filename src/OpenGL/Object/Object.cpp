#include "OpenGL/Object/Object.hpp"
#include "Debug.hpp"


namespace {

GLsizei
get_max_label_length()
{
    GLsizei ret;
    glGetIntegerv(GL_MAX_LABEL_LENGTH, &ret);
    assert(ret > 0);
    return ret;
}

}

namespace OpenGL {

Object::Object(Object::Name&& name, const GLchar* label, GLenum identifier)
        : m_name(std::move(name))
{
    // by the time an OpenGL object is constructed, OpenGL should have been
    // initialized and get_max_label_length() should work just fine.
    static GLsizei max_label_length = get_max_label_length();
    if (label) {
        m_label = std::make_unique<std::string>(label);
        if (m_label->size() <= max_label_length) {
            glObjectLabel(identifier,
                          m_name.get(),
                          m_label->size(),
                          m_label->c_str());
            // FIX when name is invalid,
            // FIX GL_INVALID_VALUE instead of GL_INVALID_OPERATION is reported.
        } else {
            m_label.reset();
        }
    }

}

}

