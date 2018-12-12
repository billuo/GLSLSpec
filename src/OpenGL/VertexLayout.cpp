/**
 * @File VertexLayout.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Utility/Debug.hpp>
#include <OpenGL/Constants.hpp>
#include <OpenGL/VertexLayout.hpp>


using Usage = OpenGL::VertexAttribute::Usage;
DEFINE_ENUMERATION_DATABASE(Usage) {{Usage::Position, "position"},
                                    {Usage::Color,    "color"},
                                    {Usage::Normal,   "normal"},
                                    {Usage::TexCoord, "texture coordinate"},
                                    {Usage::Tangent,  "tangent"},
                                    {Usage::Other,    "others"}};

namespace OpenGL {

VertexAttribute::VertexAttribute(Usage usage, GLint size, GLenum data_type, const std::string& name)
{
    if (name.empty()) {
        this->name = "v_" + E<Usage>(usage).to_string();
    }
    this->usage = usage;
    this->size = size;
    this->data_type = data_type;
    this->stride = size * sizeOfDataType(data_type);
}

void
VertexLayout::enable(GLuint index)
{
    m_vao.bind();
    glEnableVertexAttribArray(index);
}

void
VertexLayout::disable(GLuint index)
{
    m_vao.bind();
    glDisableVertexAttribArray(index);
}

void
VertexLayout::bind_buffer(const Buffer& buffer, Usage usage)
{
    auto& attr = attribute(usage);
    if (!attr) {
        Log::e("Attribute with which to bind a buffer not defined.");
        return;
    }
    m_vao.bind();
    glBindVertexBuffer(underlying_cast(usage), buffer.name(), attr->offset, attr->stride);
}

void
VertexLayout::attribute_binding(GLuint attribute_index, GLuint binding_index)
{
    m_vao.bind();
    glVertexAttribBinding(attribute_index, binding_index);
}

// void
// VertexLayout::bind_buffer(const Buffer& buffer, const VertexAttribute& attribute, bool enabled)
// {
//     define(attribute);
//     bind_buffer(buffer, attribute.usage);
//     if (enabled) {
//         enable(attribute.index);
//     } else {
//         disable(attribute.index);
//     }
//     attribute_binding(attribute.index, underlying_cast(attribute.usage));
// }

} // namespace OpenGL
