#ifndef OPENGL_LAB_TRIGONOMETRIC_HPP
#define OPENGL_LAB_TRIGONOMETRIC_HPP

#include "glm/trigonometric.hpp"
#include <ostream>


namespace Math {

class Degree;

class Radian : public Real {
  public:
    using Real::v;

    class Tag {};

    constexpr Radian(Tag, float value) : Real(value)
    {}

    Degree degrees() const;

    constexpr bool operator==(const Radian& rhs) const
    { return v == rhs.v; }

    constexpr bool operator!=(const Radian& rhs) const
    { return !(rhs == *this); }

    constexpr bool operator<(const Radian& rhs) const
    { return v < rhs.v; }

    constexpr bool operator>(const Radian& rhs) const
    { return rhs < *this; }

    constexpr bool operator<=(const Radian& rhs) const
    { return !(rhs < *this); }

    constexpr bool operator>=(const Radian& rhs) const
    { return !(*this < rhs); }

    constexpr Radian& operator+()
    { return *this; }

    constexpr const Radian& operator+() const
    { return *this; }

    constexpr Radian operator-() const
    { return Radian(Tag(), -v); }

    constexpr Radian& operator+=(const Radian& rhs)
    {
        v += rhs.v;
        return *this;
    }

    constexpr Radian& operator-=(const Radian& rhs)
    {
        v -= rhs.v;
        return *this;
    }

    template <typename T>
    constexpr Radian& operator*=(T&& x)
    {
        v *= x;
        return *this;
    }

    template <typename T>
    constexpr Radian& operator/=(T&& x)
    {
        v /= x;
        return *this;
    }

    constexpr Radian operator+(const Radian& rhs) const
    {
        Radian ret(*this);
        return ret += rhs;
    }

    constexpr Radian operator-(const Radian& rhs) const
    {
        Radian ret(*this);
        return ret -= rhs;
    }

    template <typename T>
    constexpr Radian operator*(T&& x) const
    {
        Radian ret(*this);
        return ret *= x;
    }

    template <typename T>
    constexpr Radian operator/(T&& x) const
    {
        Radian ret(*this);
        return ret /= x;
    }

    friend std::ostream& operator<<(std::ostream& os, const Radian& radian)
    { return os << radian.v << " rad"; }

    float sin() const
    { return glm::sin(v); }

    float cos() const
    { return glm::cos(v); }

    float tan() const
    { return glm::tan(v); }

    float asin() const
    { return glm::asin(v); }

    float acos() const
    { return glm::acos(v); }

    float atan() const
    { return glm::atan(v); }

    float sinh() const
    { return glm::sinh(v); }

    float cosh() const
    { return glm::cosh(v); }

    float tanh() const
    { return glm::tanh(v); }

    float asinh() const
    { return glm::asinh(v); }

    float acosh() const
    { return glm::acosh(v); }

    float atanh() const
    { return glm::atanh(v); }

    /// round to [0, 2Pi)
    void round()
    {
        v = std::fmod(v, 2 * Pi);
        if (v < 0) {
            v += 2 * Pi;
        }
    }

    /// round to [-Pi, Pi)
    void round_half()
    {
        v += Pi;
        round();
        v -= Pi;
    }
};

class Degree : public Real {
  public:
    using Real::v;

    class Tag {};

    constexpr Degree(Tag, float value) : Real(value)
    {}

    Radian radians() const;

    constexpr bool operator==(const Degree& rhs) const
    { return v == rhs.v; }

    constexpr bool operator!=(const Degree& rhs) const
    { return !(rhs == *this); }

    constexpr bool operator<(const Degree& rhs) const
    { return v < rhs.v; }

    constexpr bool operator>(const Degree& rhs) const
    { return rhs < *this; }

    constexpr bool operator<=(const Degree& rhs) const
    { return !(rhs < *this); }

    constexpr bool operator>=(const Degree& rhs) const
    { return !(*this < rhs); }

    constexpr const Degree& operator+() const
    { return *this; }

    constexpr Degree& operator+()
    { return *this; }

    constexpr Degree operator-() const
    { return Degree(Tag(), -v); }

    constexpr Degree& operator+=(const Degree& rhs)
    {
        v += rhs.v;
        return *this;
    }

    constexpr Degree& operator-=(const Degree& rhs)
    {
        v -= rhs.v;
        return *this;
    }

    template <typename T>
    constexpr Degree& operator*=(T&& x)
    {
        v *= x;
        return *this;
    }

    template <typename T>
    constexpr Degree& operator/=(T&& x)
    {
        v /= x;
        return *this;
    }

    constexpr Degree operator+(const Degree& rhs) const
    {
        Degree ret(*this);
        return ret += rhs;
    }

    constexpr Degree operator-(const Degree& rhs) const
    {
        Degree ret(*this);
        return ret -= rhs;
    }

    template <typename T>
    constexpr Degree operator*(T&& x) const
    {
        Degree ret(*this);
        return ret *= x;
    }

    template <typename T>
    constexpr Degree operator/(T&& x) const
    {
        Degree ret(*this);
        return ret /= x;
    }

    friend std::ostream& operator<<(std::ostream& os, const Degree& degree)
    { return os << degree.v << " degree"; }

    float sin() const
    { return radians().sin(); }

    float cos() const
    { return radians().cos(); }

    float tan() const
    { return radians().tan(); }

    float asin() const
    { return radians().asin(); }

    float acos() const
    { return radians().acos(); }

    float atan() const
    { return radians().atan(); }

    float sinh() const
    { return radians().sinh(); }

    float cosh() const
    { return radians().cosh(); }

    float tanh() const
    { return radians().tanh(); }

    float asinh() const
    { return radians().asinh(); }

    float acosh() const
    { return radians().acosh(); }

    float atanh() const
    { return radians().atanh(); }

    /// round to [0, 360)
    void round()
    {
        v = std::fmod(v, 360.0f);
        if (v < 0) {
            v += 360.0f;
        }
    }

    /// round to [-180, 180)
    void round_half()
    {
        v += 180.0f;
        round();
        v -= 180.0f;
    }
};

inline Degree
Radian::degrees() const
{ return {Degree::Tag(), v * 180.0f / Pi}; }

inline Radian
Degree::radians() const
{ return {Radian::Tag(), v / 180.0f * Pi}; }

inline float
sin(Radian rad)
{ return rad.sin(); }

inline float
sin(Degree deg)
{ return deg.sin(); }

inline float
cos(Radian rad)
{ return rad.cos(); }

inline float
cos(Degree deg)
{ return deg.cos(); }

inline float
tan(Radian rad)
{ return rad.tan(); }

inline float
tan(Degree deg)
{ return deg.tan(); }

inline float
asin(Radian rad)
{ return rad.asin(); }

inline float
asin(Degree deg)
{ return deg.asin(); }

inline float
acos(Radian rad)
{ return rad.acos(); }

inline float
acos(Degree deg)
{ return deg.acos(); }

inline float
atan(Radian rad)
{ return rad.atan(); }

inline float
atan(Degree deg)
{ return deg.atan(); }

inline float
sinh(Radian rad)
{ return rad.sinh(); }

inline float
sinh(Degree deg)
{ return deg.sinh(); }

inline float
cosh(Radian rad)
{ return rad.cosh(); }

inline float
cosh(Degree deg)
{ return deg.cosh(); }

inline float
tanh(Radian rad)
{ return rad.tanh(); }

inline float
tanh(Degree deg)
{ return deg.tanh(); }

inline float
asinh(Radian rad)
{ return rad.asinh(); }

inline float
asinh(Degree deg)
{ return deg.asinh(); }

inline float
acosh(Radian rad)
{ return rad.acosh(); }

inline float
acosh(Degree deg)
{ return deg.acosh(); }

inline float
atanh(Radian rad)
{ return rad.atanh(); }

inline float
atanh(Degree deg)
{ return deg.atanh(); }

} // namespace Math

inline constexpr Math::Radian
operator "" _rad(long double n)
{ return {Math::Radian::Tag(), static_cast<float>(n)}; }

inline constexpr Math::Radian
operator "" _rad(unsigned long long int n)
{ return {Math::Radian::Tag(), static_cast<float>(n)}; }

inline constexpr Math::Degree
operator "" _deg(long double n)
{ return {Math::Degree::Tag(), static_cast<float>(n)}; }

inline constexpr Math::Degree
operator "" _deg(unsigned long long int n)
{ return {Math::Degree::Tag(), static_cast<float>(n)}; }

#endif //OPENGL_LAB_TRIGONOMETRIC_HPP
