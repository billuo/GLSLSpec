/**
 * @File VertexLayout.hpp
 * @author Zhen Luo 461652354@qq.com
 */
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

    /// @note The parameter of this ctor are the only ones that must be specified by user.
    /// In that case, the attribute name is figured out reasonably and
    /// it's the shader writers' responsibility to obey the naming rules.
    /// Other properties either has a reasonable default or can be obtained from shader program at runtime.
    explicit VertexAttribute(Usage usage, GLint size, GLenum data_type, const std::string& name = "");

    /// Name in shader, used to retrieve the index etc.
    std::string name{};
    /// User defined type of usage.
    Usage usage{Usage::Other};
    /// Index in shader
    /// @note It's the first argument passed to glVertexArrayFormat().
    GLuint index{-1u};
    /// Number of components this attribute has.
    GLint size{-1};
    /// Data type of each component that are stored in client memory.
    /// @note For instance, GL_UNSIGNED_BYTE or GL_FLOAT.
    GLenum data_type{GL_FALSE};
    /// If GL_TRUE, integer data_type will be normalized while being uploaded to range [-1,1] or [0,1].
    GLboolean normalized{GL_FALSE};
    /// Relative offset of the first element of such attribute to the beginning of buffer supplying the data.
    GLuint offset{0};
    /// Distance between elements within the buffer.
    /// It defaults to 0 which means OpenGL will calculate it based on size and data_type.
    GLuint stride{0};
};

class VertexLayout {
    using Usage = VertexAttribute::Usage;
  public:
    VertexLayout() = default;

    VertexLayout(std::initializer_list<VertexAttribute> attribute_list)
    { define_range(attribute_list.begin(), attribute_list.end()); }

    VertexLayout(VertexLayout&&) = default;
    VertexLayout& operator=(VertexLayout&&) = default;

    ~VertexLayout()
    { clear(); }

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

    /// @brief Release all resources.
    /// @note It only deletes, but not unbinds the VAO it owns.
    void clear();

    /// @brief Enable a vertex attribute as an attribute array.
    /// @param index The index of the attribute.
    void enable(GLuint index);

    /// @brief Disable a vertex attribute as an attribute array.
    /// @param index The index of the attribute.
    void disable(GLuint index);

    /// @brief Bind the underlying VAO to current OpenGL context.
    void bind() const
    { m_vao.bind(); }

    /// @brief Bind a vertex buffer as data source of a vertex attribute.
    /// @param buffer The vertex buffer to bind.
    /// @param usage The specified usage, used to fetch the attribute already defined in this layout.
    /// @note It only binds a buffer to a buffer binding point,
    /// which should be later bound to vertex attribute indices with attribute_binding().
    void bind_buffer(const Buffer& buffer, Usage usage);

    /// @brief Bind a vertex buffer as data source of the vertex attribute
    /// @param buffer The vertex buffer to bind.
    /// @param attribute The vertex attribute to bind to. It replaces the already defined one, if any.
    /// @param enabled Defaults to true, which means upon binding finished the attribute will also be enabled as an array.
    /// @note It calls attribute_binding() to bind the binding point for @p attribute.usage to @p attribute.index.
    void bind_buffer(const Buffer& buffer, const VertexAttribute& attribute, bool enabled = true);

    /// @brief Bind a binding point to an attribute index.
    /// @param attribute_index The index of the attribute.
    /// @param binding_index The index of the binding point.
    void attribute_binding(GLuint attribute_index, GLuint binding_index);

  private:
    /// The actual vertex layout OpenGL sees.
    VertexArray m_vao{};

    /// Attributes each of a specific usage.
    /// @note Indices of attributes in the array is exactly the index of the binding point they will be bound to.
    /// @sa VertexAttribute::Usage
    std::array<Shared<const VertexAttribute>, underlying_cast(Usage::Max)> m_attributes{};
};

} // namespace OpenGL


