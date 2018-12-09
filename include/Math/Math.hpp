#pragma once

#define GLM_ENABLE_EXPERIMENTAL 1

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <algorithm>
#include <type_traits>
#include <ostream>


namespace glm {

/// Compare two numbers for near equality.
template <typename T>
GLM_FUNC_DECL GLM_INLINE bool
nearlyEqual(const T x, const T y)
{ return abs(x - y) <= T(1) / T(1024); }

/// Compare two vec for near equality.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
nearlyEqual(vec<L, T, Q> const& x, vec<L, T, Q> const& y)
{
    for (length_t i = 0; i < L; ++i) {
        if (!nearlyEqual(x[i], y[i])) {
            return false;
        }
    }
    return true;
}

template <typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
nearlyEqual(tquat<T, Q> const& q1, tquat<T, Q> const& q2)
{
    return nearlyEqual(q1.x, q2.x) && nearlyEqual(q1.y, q2.y) && nearlyEqual(q1.z, q2.z) && nearlyEqual(q1.w, q2.w);
}

/// Compare two mat for near equality.
template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL bool
nearlyEqual(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y)
{
    for (length_t i = 0; i < C; ++i) {
        for (length_t j = 0; j < R; ++j) {
            if (!nearlyEqual(x[i][j], y[i][j])) {
                return false;
            }
        }
    }
    return true;
}

/// Test vec against identity
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
isIdentity(vec<L, T, Q> const& x)
{ return nearlyEqual(x, identity<vec<L, T, Q> >()); }

template <typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
isIdentity(tquat<T, Q> const& q)
{ return nearlyEqual(q, glm::identity<tquat<T, Q>>()); }

/// Test mat against identity
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
isIdentity(mat<L, L, T, Q> const& x)
{ return nearlyEqual(x, identity<mat<L, L, T, Q> >()); }

/// Test vec against zero
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
isZero(vec<L, T, Q> const& x)
{ return nearlyEqual(x, zero<vec<L, T, Q> >()); }

/// Test mat against zero
template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
isZero(mat<C, R, T, Q> const& x)
{ return nearlyEqual(x, zero<mat<C, R, T, Q> >()); }

/// Always false for non square matrix.
template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL bool
isDiagonal(mat<C, R, T, Q> const& x)
{ return false; }

/// True if mat is diagonal.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL bool
isDiagonal(mat<L, L, T, Q> const& x)
{
    for (length_t i = 0; i < L; ++i) {
        for (length_t j = 0; j < L; ++j) {
            if (i != j && !nearlyEqual(x[i][j], T(0))) {
                return false;
            }
        }
    }
    return true;
}

/// True if vec is normal (length(vec) == 1)
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool
isNormal(vec<L, T, Q> const& x)
{ return nearlyEqual(length(x), T(1)); }

template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE T
prod(vec<L, T, Q> const& x)
{
    T ret = x[0];
    for (length_t i = 1; i < L; ++i) {
        ret *= x[i];
    }
    return ret;
}

template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL std::ostream&
operator<<(std::ostream& os, vec<L, T, Q> const& x)
{
    os << '{' << x[0];
    for (length_t i = 1; i < L; ++i) {
        os << ", " << x[i];
    }
    os << '}';
    return os;
}

template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL std::ostream&
operator<<(std::ostream& os, mat<C, R, T, Q> const& x)
{
    os << "{\n";
    for (length_t i = 0; i < x.length(); ++i) {
        os << std::string("  ") << glm::row(x, i) << '\n';
    }
    os << "}";
    return os;
}

} // namespace glm

// TODO Re-encapsulate into own Vector and Metrix classes.
namespace Math {

static constexpr float Pi = 3.1415926f;

struct Real {
    using value_type = float;
    value_type v;

    constexpr Real(value_type value = 0.0f) : v(value)
    {}

    Real& operator=(value_type value)
    { return assign(value); }

    explicit constexpr operator value_type() const
    { return v; }

    Real& assign(value_type value)
    {
        v = value;
        return *this;
    }

    // TODO many operators


    // TODO test normalize()

    /// normalize to (-mag, mag)
    Real& normalize(value_type mag)
    { return assign(std::fmod(v, mag)); }

    /// normalize to (min, max)
    Real& normalize(value_type min, value_type max)
    {
        if (min >= max) {
            return assign(0);
        }
        auto mid = (min + max) / 2;
        return assign(std::fmod(v - mid, max - mid) + mid);
    }

    Real& clamp(value_type mag)
    { return assign(std::clamp(v, -mag, mag)); }

    Real& clamp(value_type min, float max)
    { return assign(std::clamp(v, min, max)); }

};

template <typename T>
T
sign(T x)
{
    static_assert(std::is_arithmetic<T>::value);
    static_assert(std::numeric_limits<T>::is_signed);
    return std::signbit(x) ? T(-1) : T(1);
}

} // namespace Math

#include "Math/Trigonometric.hpp"

