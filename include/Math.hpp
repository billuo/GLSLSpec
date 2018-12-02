#ifndef MATH_HPP_AUWT35Z6
#define MATH_HPP_AUWT35Z6

#define GLM_ENABLE_EXPERIMENTAL 1

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include <type_traits>
#include <sstream>
#include <iomanip>


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
    return nearlyEqual(q1.x, q2.x) && nearlyEqual(q1.y, q2.y) && nearlyEqual(q1.z, q2.z) &&
           nearlyEqual(q1.w, q2.w);
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
    os << std::setprecision(2) << std::fixed;
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
    os << std::setprecision(2) << std::fixed;
    auto x_t = glm::transpose(x);
    os << "{\n";
    for (length_t i = 0; i < x_t.length(); ++i) {
        os << std::string("  ") << x_t[i] << '\n';
    }
    os << "}";
    return os;
}

template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL std::string
string_cast(vec<L, T, Q> const& x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL std::string
string_cast(mat<C, R, T, Q> const& x)
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

} // namespace glm

namespace Math {

template <typename T>
T
sign(T x)
{
    static_assert(std::is_arithmetic<T>::value, "");
    static_assert(std::numeric_limits<T>::is_signed, "");
    return std::signbit(x) ? T(-1) : T(1);
}
} // namespace Math

static const float Pi = 3.1415926f;

static inline float
RadianOfDegree(float x)
{ return x / 180.0f * Pi; }

static inline float
DegreeOfRadian(float x)
{ return x * 180.0f / Pi; }

#endif /* end of include guard: MATH_HPP_AUWT35Z6 */
