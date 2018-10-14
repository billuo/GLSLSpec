#ifndef MATH_HPP_AUWT35Z6
#define MATH_HPP_AUWT35Z6

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/string_cast.hpp>

namespace glm {

/// Compare two numbers for near equality.
template <typename T>
GLM_FUNC_DECL GLM_INLINE bool nearlyEqual(const T x, const T y) {
    return x == y;
}
template <>
GLM_FUNC_DECL GLM_INLINE bool nearlyEqual<float>(const float x, const float y) {
    return abs(x - y) < 0.001f;
}
template <>
GLM_FUNC_DECL GLM_INLINE bool nearlyEqual<double>(const double x, const double y) {
    return abs(x - y) < 0.001;
}

/// Compare two vec/mat for near equality.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL bool nearlyEqual(vec<L, T, Q> const& x, vec<L, T, Q> const& y) {
    for (length_t i = 0; i < L; ++i) {
        if (!nearlyEqual(x[i], y[i])) {
            return false;
        }
    }
    return true;
}

/// Compare two vec/mat for near equality.
template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL bool nearlyEqual(mat<C, R, T, Q> const& x, mat<C, R, T, Q> const& y) {
    for (length_t i = 0; i < C; ++i) {
        for (length_t j = 0; j < R; ++j) {
            if (!nearlyEqual(x[i][j], y[i][j])) {
                return false;
            }
        }
    }
    return true;
}

/// Test against identity vec/mat.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool isIdentity(vec<L, T, Q> const& x) {
    return nearlyEqual(x, identity<vec<L, T, Q> >());
}
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool isIdentity(mat<L, L, T, Q> const& x) {
    return nearlyEqual(x, identity<mat<L, L, T, Q> >());
}

/// Test against zero vec/mat.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool isZero(vec<L, T, Q> const& x) {
    return nearlyEqual(x, zero<vec<L, T, Q> >());
}
template <length_t C, length_t R, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool isZero(mat<C, R, T, Q> const& x) {
    return nearlyEqual(x, zero<mat<C, R, T, Q> >());
}

/// Test mat for disagonality.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL bool isDiagonal(mat<L, L, T, Q> const& x) {
    for (length_t i = 0; i < L; ++i) {
        for (length_t j = 0; j < L; ++j) {
            if (i != j && !nearlyEqual(x[i][j], T(0))) {
                return false;
            }
        }
    }
    return true;
}

/// Test vec against normalized version.
template <length_t L, typename T, qualifier Q>
GLM_FUNC_DECL GLM_INLINE bool normalized(vec<L, T, Q> const& x) {
    return nearlyEqual(length(x), T(1));
}

} // namespace glm

static const float Pi = 3.1415926f;
static inline float RadianOfDegree(float x) { return x / 180.0f * Pi; }
static inline float DegreeOfRadian(float x) { return x * 180.0f / Pi; }

#endif /* end of include guard: MATH_HPP_AUWT35Z6 */
