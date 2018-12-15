/**
 * @file VertexBuffer.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Buffer.hpp>
#include <OpenGL/VertexAttribute.hpp>


namespace OpenGL {

/// @brief Define a OpenGL buffer object for storing vertex attributes data only.
/// @tparam T Value type it's provided.
template <typename T>
class VertexBuffer : public Buffer {
    using Usage = VertexAttribute::Usage;
  public:
    explicit VertexBuffer(Usage usage) : m_usage(usage)
    {}

    /// @brief Copy an attribute value to cache.
    /// @param value The value to copy.
    void add(const T& value)
    { m_data.push_back(value); }

    /// @brief Move an attribute value to cache.
    /// @param value The value to move.
    void add(T&& value)
    { m_data.emplace_back(std::move(value)); }

    /// @brief Upload the data cached to OpenGL server.
    /// @note After uploading, the local cache becomes empty and memory is released.
    void upload()
    {
        Buffer::Bind(GL_ARRAY_BUFFER, *this);
        Buffer::Data(GL_ARRAY_BUFFER, m_data.size() * sizeof(T), m_data.data(), GL_STATIC_DRAW);
        decltype(m_data) empty;
        m_data.swap(empty);
    }

    auto usage() const
    { return m_usage; }

    auto size() const
    { return m_data.size(); }

  private:
    /// Binding point index in glBindVertexBuffer
    Usage m_usage;
    std::vector<T> m_data; // empty if data already uploaded
};

} // namespace OpenGL


