/**
 * @file OpenGL/Object.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Utility/Log.hpp>
#include <OpenGL/Common.hpp>
#include <OpenGL/Debug.hpp>

#include <memory>
#include <ostream>
#include <vector>


namespace OpenGL {

/// Base class of various OpenGL objects, owning a Name and an optional label string.
class Object {
  protected:
    /**
     * @class Name
     * @brief RAII for name handles of OpenGL objects.
     * @note Not intended to be used directly!
     */
    class Name {
      public:
        explicit Name(GLuint name = 0) : m_name(name)
        {}

        Name(const Name&) = delete;

        Name& operator=(const Name&) = delete;

        Name(Name&& obj) noexcept : m_name(obj.m_name)
        { obj.clear(); }

        Name& operator=(Name&& rhs) noexcept
        {
            GLuint name = rhs.m_name;
            rhs.clear();
            m_name = name;
            return *this;
        }

        ~Name()
        { clear(); }

        GLuint get() const
        { return m_name; }

        void clear()
        { m_name = 0; }

        explicit operator GLuint() const
        { return m_name; }

        friend std::ostream& operator<<(std::ostream& os, const Object::Name& name)
        { return os << name.get(); }

      private:
        GLuint m_name;
    };

    template <typename F1, typename F2>
    class NamePool;

    template <typename F1, typename F2>
    static Object::NamePool<F1, F2> make_pool(F1 f1, F2 f2)
    {
        return Object::NamePool<F1, F2>(f1, f2);
    }

    Name m_name;
    std::unique_ptr<std::string> m_label;

  public:
    Object() = default;
    Object(Name&& name);

    GLuint name() const
    { return m_name.get(); }

    std::string label() const
    { return m_label ? *m_label : std::string(); }

    #if DEBUG_BUILD
    void label(std::string str, GLenum identifier);
    #else
    void label(std::string str, GLenum identifier){}
    #endif

    friend std::ostream& operator<<(std::ostream& os, const Object& object)
    { return os << object.label() << '#' << object.name(); }

};

/**
 * @brief Name pool for standard OpenGL objects.
 * @tparam F1 Invokable type given (GLsizei n, GLuint* ptr), populates array pointed by ptr of size n with names.
 * @tparam F2 Invokable type given (GLsizei n, GLuint* ptr), deletes names within the array pointed by ptr of size n.
 */
template <typename F1, typename F2>
class Object::NamePool {
  public:
    NamePool(F1 create_n, F2 delete_n) noexcept
            : m_create_n(create_n), m_delete_n(delete_n)
    {}

    /// Get a single name.
    Name get()
    {
        if (m_pool.empty()) {
            Refill();
        }
        Name ret(m_pool.back());
        m_pool.pop_back();
        return ret;
    }

    /// Put a single name.
    void put(Name&& name)
    {
        Name moved = std::move(name);
        m_delete.push_back(moved.get());
    }

  private:
    void Refill()
    {
        Delete();
        size_t old_size = m_pool.size();
        size_t new_size = std::max(16lu, old_size * 7 / 4);
        m_pool.resize(new_size);
        m_create_n(new_size - old_size, &m_pool[old_size]);
    }

    void Delete()
    {
        m_delete_n(m_delete.size(), m_delete.data());
        m_delete.clear();
    }

    void ClearAll()
    {
        Delete();
        m_delete_n(m_pool.size(), m_pool.data());
        m_pool.clear();
    }

    F1 m_create_n;
    F2 m_delete_n;
    std::vector<GLuint> m_pool; ///< names available
    std::vector<GLuint> m_delete; ///< names flagged to delete
};

} // namespace OpenGL

