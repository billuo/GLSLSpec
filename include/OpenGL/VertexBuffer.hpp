/**
 * @file VertexBuffer.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

#include "Object/Buffer.hpp"
#include "VertexAttribute.hpp"


namespace OpenGL {

/// @brief Define a OpenGL buffer object for storing vertex attributes data only.
/// @tparam T Value type it's provided.
template <typename T>
class VertexBuffer : public Buffer {
    using Usage = VertexAttribute::Usage;
  public:
    /// @brief Perform RAII on a mapped pointer from a VertexBuffer to avoid double mapping/unmapping.
    /// @details Also, an iterator-like interface is implemented, trying to make it as easy to use as raw pointers.
    /// @note It's not copyable so much of the common pointer arithmetic are not supported.
    /// e.g. addition/subtraction, post self-increment/decrement
    /// @warning Don't use the mapped buffer until this mapped pointer goes out of scope and automatically unmap the buffer.
    /// Thus don't prolong its life time more than necessary.
    struct MappedPtr {
        MappedPtr() = default;

        explicit MappedPtr(VertexBuffer& buffer, GLenum access = GL_READ_WRITE) : m_owner(&buffer)
        {
            if (!m_owner->m_mapped.exchange(true)) {
                m_owner->bind(GL_ARRAY_BUFFER);
                m_ptr = static_cast<T*>(Buffer::Map(GL_ARRAY_BUFFER, access));
                if (m_ptr == nullptr) {
                    m_owner->m_mapped.exchange(false);
                }
            }
            // possible: m_ptr == nullptr
        }

        ~MappedPtr()
        {
            if (m_ptr) {
                m_ptr = nullptr;
                m_owner->bind(GL_ARRAY_BUFFER);
                Buffer::Unmap(GL_ARRAY_BUFFER);
                m_owner->m_mapped.exchange(false);
            }
        }

        MappedPtr(const MappedPtr&) = delete;
        MappedPtr& operator=(const MappedPtr&) = delete;

        MappedPtr(MappedPtr&& obj) noexcept
        {
            m_owner = obj.m_owner;
            m_ptr = obj.m_ptr;
            obj.m_ptr = nullptr;
        }

        MappedPtr& operator=(MappedPtr&& rhs) noexcept
        {
            MappedPtr other(std::move(rhs));
            swap(other);
            return *this;
        }

        void swap(MappedPtr& obj) noexcept
        {
            std::swap(m_owner, obj.m_owner);
            std::swap(m_ptr, obj.m_ptr);
        }

        MappedPtr& operator+=(std::size_t n) noexcept
        {
            m_ptr += n;
            return *this;
        }

        MappedPtr& operator-=(std::size_t n) noexcept
        {
            m_ptr -= n;
            return *this;
        }

        MappedPtr& operator++() noexcept
        {
            m_ptr++;
            return *this;
        }

        MappedPtr& operator--() noexcept
        {
            m_ptr++;
            return *this;
        }

        T& operator*()
        { return *m_ptr; }

        const T& operator*() const
        { return *m_ptr; }

        T& operator[](std::size_t n)
        { return m_ptr[n]; }

        explicit operator bool() const noexcept
        { return m_ptr != nullptr; }

        bool operator==(nullptr_t) const noexcept
        { return m_ptr == nullptr; }

        bool operator!=(nullptr_t) const noexcept
        { return m_ptr != nullptr; }

        bool operator<(const MappedPtr& rhs) const noexcept
        { return m_ptr < rhs.m_ptr; }

        bool operator>(const MappedPtr& rhs) const noexcept
        { return rhs < *this; }

        bool operator<=(const MappedPtr& rhs) const noexcept
        { return !(rhs < *this); }

        bool operator>=(const MappedPtr& rhs) const noexcept
        { return !(*this < rhs); }

      private:
        VertexBuffer* m_owner{nullptr};
        T* m_ptr{nullptr};
    };

    friend struct MappedPtr;

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
    /// @details After uploading, the local cache becomes empty and memory is released, the old buffer data storage is also orphaned.
    void upload()
    {
        data(m_data.size() * sizeof(T), m_data.data(), GL_STATIC_DRAW);
        decltype(m_data) empty;
        m_data.swap(empty);
    }

    void data(GLsizeiptr size, const GLvoid* data, GLenum usage)
    {
        Buffer::Bind(GL_ARRAY_BUFFER, *this);
        Buffer::Data(GL_ARRAY_BUFFER, size, data, usage);
    }

    /// @brief Map this buffer after its data storage has been specified. Has no effect if already mapped.
    /// @return A mapped pointer for R/W. If already mapped, empty pointer.
    MappedPtr map()
    { return MappedPtr(*this); }

    auto usage() const
    { return m_usage; }

    auto size() const
    { return m_data.size(); }

  private:
    /// Binding point index in glBindVertexBuffer
    Usage m_usage;
    /// Data cached in CPU memory.
    std::vector<T> m_data;
    /// True if buffer is already mapped.
    std::atomic_bool m_mapped = false;
};

} // namespace OpenGL


