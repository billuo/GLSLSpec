/**
 * @File VertexLayout.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Utility/Debug.hpp>
#include <OpenGL/VertexLayout.hpp>


using Usage = OpenGL::VertexAttribute::Usage;
DEFINE_ENUMERATION_DATABASE(Usage) {{Usage::Position, "position"},
                                    {Usage::Color,    "color"},
                                    {Usage::Normal,   "normal"},
                                    {Usage::TexCoord, "texture coordinate"},
                                    {Usage::Tangent,  "tangent"},
                                    {Usage::Other,    "others"}};

namespace OpenGL {

template <typename Container>
static void
check_attribute_index(const Container& c, GLuint index, const std::string& when)
{
    #if DEBUG_BUILD
    using V = typename std::iterator_traits<typename Container::iterator>::value_type;
    static_assert(std::is_convertible_v<V, Shared<const VertexAttribute>>);
    auto it = std::find_if(c.begin(), c.end(), [index](const V& v)
    { return v && v->index == index; });
    if (it == c.end()) {
        DEBUG("index={} doesn't exist when {}", when);
    }
    #endif
}

VertexAttribute::VertexAttribute(Usage usage, GLint size, GLenum data_type, const std::string& name)
{
    if (name.empty()) {
        this->name = "v_" + E<Usage>(usage).to_string();
    }
    this->usage = usage;
    this->size = size;
    this->data_type = data_type;
}

void
VertexLayout::enable(GLuint index)
{
    check_attribute_index(m_attributes, index, "enable attribute");
    m_vao.bind();
    glEnableVertexAttribArray(index);
}

void
VertexLayout::disable(GLuint index)
{
    check_attribute_index(m_attributes, index, "disable attribute");
    m_vao.bind();
    glDisableVertexAttribArray(index);
}

void
VertexLayout::bind_buffer(const Buffer& buffer, Usage usage)
{
    auto& attr = attribute(usage);
    if (attr == nullptr) {
        Log::e("Attribute with which to bind a buffer not defined.");
        return;
    }
    m_vao.bind();
    glBindVertexBuffer(underlying_cast(usage), buffer.name(), attr->offset, attr->stride);
}

void
VertexLayout::attribute_binding(GLuint attribute_index, GLuint binding_index)
{
    check_attribute_index(m_attributes, attribute_index, "attribute binding point");
    m_vao.bind();
    glVertexAttribBinding(attribute_index, binding_index);
}

void
VertexLayout::bind_buffer(const Buffer& buffer, const VertexAttribute& attribute, bool enabled)
{
    define(attribute);
    bind_buffer(buffer, attribute.usage);
    if (enabled) {
        enable(attribute.index);
    } else {
        disable(attribute.index);
    }
    attribute_binding(attribute.index, underlying_cast(attribute.usage));
}

void
VertexLayout::clear()
{
    for (auto& p : m_attributes) {
        p.reset();
    }
    m_vao.~VertexArray();
}

} // namespace OpenGL
