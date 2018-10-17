/**
 * @file OpenGL/Object.hpp
 * @brief Base class representing generic OpenGL objects, each contains a name of type GLuint of OpenGL object.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef OBJECT_HPP_NPLQXK1F
#define OBJECT_HPP_NPLQXK1F
#pragma once

#include "OpenGL/Common.hpp"
#include "Utility.hpp"
#include <vector>

namespace OpenGL {

/// Base class of various OpenGL objects, owning a Name and an optional label.
class Object {
protected:
    /**
     * @class Name
     * @brief RAII for name handles of OpenGL objects.
     * @note Not intended to be used directly!
     */
    class Name {
    public:
        explicit Name(GLuint name = 0) : m_name(name) {}
        Name(const Name&) = delete;
        Name& operator=(const Name&) = delete;
        Name(Name&& obj) : m_name(obj.m_name) { obj.clear(); }
        Name& operator=(Name&& rhs) {
            GLuint name = rhs.m_name;
            rhs.clear();
            m_name = name;
            return *this;
        }
        ~Name() { clear(); }

        GLuint get() const { return m_name; }
        void clear() { m_name = 0; }

        operator GLuint() const { return m_name; }

    private:
        GLuint m_name;
    };

    template <typename F1, typename F2>
    class NamePool;
    template <typename F1, typename F2>
    static Object::NamePool<F1, F2> MakeNamePool(F1 f1, F2 f2) {
        return NamePool<F1, F2>(f1, f2);
    }

public:
    Object() = default;
    Object(Name&& name) : m_name(std::move(name)), m_label() {}
    Object(Name&& name, const std::string& label) : m_name(std::move(name)), m_label(new std::string(label)) {}

    GLuint name() const { return m_name.get(); }

protected:
    Name m_name;
    std::unique_ptr<std::string> m_label;
};

/**
 * @brief Name pool for standard OpenGL objects.
 * @tparam F1 Invokable type given (GLsizei n, GLuint* ptr), popullates array pointed by ptr of size n with names.
 * @tparam F2 Invokable type given (GLsizei n, GLuint* ptr), deletes names within the array pointed by ptr of size n.
 */
template <typename F1, typename F2>
class Object::NamePool {
public:
    NamePool(F1 create_n, F2 delete_n) : m_create_n(create_n), m_delete_n(delete_n) {}

    /// Get a single name.
    Name Get() {
        if (Pool.empty()) {
            Refill();
        }
        Name ret(Pool.back());
        Pool.pop_back();
        return ret;
    }

    /// Put a single name.
    void Put(Name&& name) {
        Name moved = std::move(name);
        ToDelete.push_back(moved.get());
    }

    // template <typename OutputIterator>
    // static void Get(GLsizei n, OutputIterator out) {
    //     while (Pool.size() < n) {
    //         Refill();
    //     }
    //     size_t new_size = Pool.size() - n;
    //     auto it = Pool.begin() + new_size;
    //     while (it != Pool.end()) {
    //         *out++ = Name(*it++);
    //     }
    //     Pool.resize(new_size);
    // }

    // template <typename InputIterator>
    // static void Put(GLsizei n, InputIterator in) {
    //     static_assert(std::is_same<Name, std::decay_t<decltype(*in)>>::value,
    //                   "InputIterator dereferenced and decayed must return Name");
    //     while (n--) {
    //         Put(std::move(*in++));
    //     }
    // }

private:
    void Refill() {
        Delete();
        size_t old_size = Pool.size();
        size_t new_size = std::max(16lu, old_size * 7 / 4);
        Pool.resize(new_size);
        m_create_n(new_size - old_size, reinterpret_cast<GLuint*>(&Pool[old_size]));
    }

    void Delete() {
        m_delete_n(ToDelete.size(), ToDelete.data());
        ToDelete.clear();
    }

    void ClearAll() {
        Delete();
        m_delete_n(Pool.size(), reinterpret_cast<GLuint*>(Pool.data()));
        Pool.clear();
    }

    F1 m_create_n;
    F2 m_delete_n;
    std::vector<GLuint> Pool;
    std::vector<GLuint> ToDelete;
};

} // namespace OpenGL

#endif /* end of include guard: OBJECT_HPP_NPLQXK1F */
