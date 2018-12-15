/**
 * @File Object.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Object/Object.hpp>


namespace {

GLsizei
get_max_label_length()
{
    GLsizei ret;
    glGetIntegerv(GL_MAX_LABEL_LENGTH, &ret);
    return ret;
}

}

namespace OpenGL {

Object::Object(Object::Name&& name) : m_name(std::move(name))
{}

#if DEBUG_BUILD

void
Object::label(std::string str, GLenum identifier)
{
    static std::size_t max_label_length = get_max_label_length();
    if (!str.empty()) {
        m_label = std::make_unique<std::string>(std::move(str));
        if (m_label->size() <= max_label_length) {
            glObjectLabel(identifier, m_name.get(), static_cast<GLsizei>(m_label->size()), m_label->c_str());
            // XXX when name is invalid, GL_INVALID_VALUE instead of GL_INVALID_OPERATION is reported.
        } else {
            Log::w("Label {} exceeds maximum label length={}", str, max_label_length);
        }
    } else {
        m_label.reset();
        glObjectLabel(identifier, m_name.get(), 0, nullptr);
    }
}

#endif

}

