/**
 * @File VertexLayout.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Common.hpp"
#include "Object/VertexArray.hpp"
#include "VertexBuffer.hpp"
#include <Utility/Misc.hpp>
#include <Utility/Enumeration.hpp>


namespace OpenGL {

class VertexLayout : public VertexArray {
    using Usage = VertexAttribute::Usage;
  public:
    VertexLayout() = default;

    VertexLayout(std::initializer_list<VertexAttribute> attribute_list)
    { define_range(attribute_list.begin(), attribute_list.end()); }

    VertexLayout(VertexLayout&&) = default;
    VertexLayout& operator=(VertexLayout&&) = default;

    ~VertexLayout() = default;

    auto& attribute(Usage usage)
    { return attributes[underlying_cast(usage)]; }

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

    /// @brief Remove all defined attributes.
    void clear()
    {
        for (auto& attr : attributes) {
            attr.reset();
        }
    }

    /// @brief Bind a vertex buffer to a binding point as a viable data source.
    template <typename T>
    void bind_buffer(const VertexBuffer<T>& vbo)
    {
        bind();
        glBindVertexBuffer(underlying_cast(vbo.usage()), vbo.name(), 0, sizeof(T));
    }

    /// @brief
    /// @param location
    /// @param usage
    void attribute_name_me(GLuint location, Usage usage)
    {
        glEnableVertexAttribArray(location);
        glVertexAttribBinding(location, underlying_cast(usage));
        auto& attr = attribute(usage);
        glVertexAttribFormat(location, attr->size, attr->type, attr->normalized, attr->relative_offset);
    }

    /// Attributes each of a specific usage.
    /// @note Indices of attributes in the array is exactly the index of the binding point they will be bound to.
    /// @sa VertexAttribute::Usage
    std::array<Shared<const VertexAttribute>, underlying_cast(Usage::Max)> attributes{};
};

} // namespace OpenGL


