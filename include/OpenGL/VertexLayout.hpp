#pragma once

#include <OpenGL/Common.hpp>
#include <OpenGL/Object/VertexArray.hpp>
#include <OpenGL/Object/Buffer.hpp>
#include <Utility/Misc.hpp>
#include <Utility/Enumeration.hpp>

#include <string>


namespace OpenGL {

/// Define a single attribute of a vertex.
/// @note Contains both the information of actual format & element address (offset, stride), so as to keep it simple for now.
/// This actually defeats the whole purpose of separating glVertexAttribPointer into glAttribFormat + glBindVertexBuffer.
// TODO remember to adapt to the new way of specifying vertices
struct VertexAttribute {
    enum class Usage {
        Position,
        Color,
        Normal,
        TexCoord,
        Tangent,
        Other,
        Max,
    };
    /// Name in shader, used to retrieve the index etc.
    std::string name{};
    /// User defined type of usage.
    Usage usage{Usage::Other};
    /// Index in shader
    /// @note It's the first argument passed to glVertexArrayFormat().
    GLuint index{-1u};
    /// Data type stored in the memory of client.
    /// @note For instance, GL_UNSIGNED_BYTE or GL_FLOAT. NOT GL_FLOAT_VEC3 or such!
    GLenum data_type{0};
    /// If GL_TRUE, integer type should be normalized to floats in range [-1,1] or [0,1].
    GLboolean normalized{GL_FALSE};
    /// Relative offset of the first element of such attribute to the beginning of buffer supplying the data.
    GLuint offset{0};
    /// Distance between elements within the buffer
    GLuint stride{0};
};

class VertexLayout {
    using Usage = VertexAttribute::Usage;
  public:
    VertexLayout() = default;

    VertexLayout(std::initializer_list<VertexAttribute> attribute_list)
    { define_range(attribute_list.begin(), attribute_list.end()); }

    ~VertexLayout() = default;

    auto& attribute(Usage usage)
    { return m_attributes[underlying_cast(usage)]; }

    /// @brief Add an attribute into this layout.
    /// @param attr The attribute to add.
    void define(const VertexAttribute& attr)
    { attribute(attr.usage) = std::make_shared<VertexAttribute>(attr); }

    /// @brief Add a range of attributes into this layout.
    /// @tparam InputIterator Input iterator with a value_type of VertexAttribute.
    /// @param input The first element of this range.
    /// @param end The last off one element of this range.
    template <typename InputIterator>
    void define_range(InputIterator input, InputIterator end)
    {
        static_assert(std::is_same_v<std::decay_t<typename std::iterator_traits<InputIterator>::value_type>,
                VertexAttribute>);
        while (input != end) {
            auto&& attr = *input++;
            auto& my_attr = attribute(attr.usage);
            if (DEBUG_BUILD && my_attr) {
                Log::w("Vertex attribute of usage:{} already defined and will be overwritten.", E<Usage>(attr.usage));
            }
            my_attr = std::make_shared<VertexAttribute>(attr);
        }
    }

    /// @brief Enable a vertex attribute as an attribute array.
    /// @param index The index of the attribute.
    void enable(GLuint index);

    /// @brief Disable a vertex attribute as an attribute array.
    /// @param index The index of the attribute.
    void disable(GLuint index);

    /// @brief Bind a vertex buffer as data source of a vertex attribute.
    /// @param buffer The vertex buffer to bind.
    /// @param usage The specified usage, used to fetch the attribute already defined in this layout.
    void bind_buffer(const Buffer& buffer, Usage usage);

    /// @brief Bind a vertex buffer as data source of the vertex attribute
    /// @param buffer The vertex buffer to bind.
    /// @param attribute The vertex attribute to bind to. It replaces the already defined one, if any.
    /// @param enabled Defaults to true, which means upon binding finished the attribute will also be enabled as an array.
    void bind_buffer(const Buffer& buffer, const VertexAttribute& attribute, bool enabled = true)
    {
        define(attribute);
        bind_buffer(buffer, attribute.usage);
        if (enabled) {
            enable(attribute.index);
        } else {
            disable(attribute.index);
        }
    }

    void attribute_binding(GLuint attribute_index, GLuint binding_index);

  private:
    /// The actual vertex layout OpenGL sees.
    VertexArray m_vao{};

    /// Attributes each of a specific usage.
    /// @sa VertexAttribute::Usage
    std::array<Shared<VertexAttribute>, underlying_cast(Usage::Max)> m_attributes{};
};

} // namespace OpenGL


