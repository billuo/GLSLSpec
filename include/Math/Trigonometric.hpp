/**
 * @File Trigonometric.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <glm/trigonometric.hpp>
#include <ostream>


namespace Math {

class Degree;

class Radian : public Real {
  public:
    using Real::v;
    using value_type = Real::value_type;

    class Tag {};

    constexpr Radian(Tag, float value) : Real(value)
    {}

    static Radian Of(float v)
    { return Radian{Tag(), v}; }

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

    float sinh() const
    { return glm::sinh(v); }

    float cosh() const
    { return glm::cosh(v); }

    float tanh() const
    { return glm::tanh(v); }

    static Radian asin(float v)
    { return Radian{Tag(), glm::asin(v)}; }

    static Radian acos(float v)
    { return Radian{Tag(), glm::acos(v)}; }

    static Radian atan(float v)
    { return Radian{Tag(), glm::atan(v)}; }

    static Radian atan(float y, float x)
    { return Radian{Tag(), glm::atan(y, x)}; }

    static Radian asinh(float v)
    { return Radian{Tag(), glm::asinh(v)}; }

    static Radian acosh(float v)
    { return Radian{Tag(), glm::acosh(v)}; }

    static Radian atanh(float v)
    { return Radian{Tag(), glm::atanh(v)}; }

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
    using value_type = Real::value_type;

    class Tag {};

    constexpr Degree(Tag, value_type value) : Real(value)
    {}

    static Degree Of(value_type v)
    { return Degree{Tag(), v}; }

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

    float sinh() const
    { return radians().sinh(); }

    float cosh() const
    { return radians().cosh(); }

    float tanh() const
    { return radians().tanh(); }

    static Degree asin(float v)
    { return Radian::asin(v).degrees(); }

    static Degree acos(float v)
    { return Radian::acos(v).degrees(); }

    static Degree atan(float v)
    { return Radian::atan(v).degrees(); }

    static Degree atan(float y, float x)
    { return Radian::atan(y, x).degrees(); }

    static Degree asinh(float v)
    { return Radian::asinh(v).degrees(); }

    static Degree acosh(float v)
    { return Radian::acosh(v).degrees(); }

    static Degree atanh(float v)
    { return Radian::atanh(v).degrees(); }

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

} // namespace Math

inline constexpr Math::Radian
operator "" _rad(long double n)
{ return {Math::Radian::Tag(), static_cast<Math::Radian::value_type>(n)}; }

inline constexpr Math::Radian
operator "" _rad(unsigned long long int n)
{ return {Math::Radian::Tag(), static_cast<Math::Radian::value_type>(n)}; }

inline constexpr Math::Degree
operator "" _deg(long double n)
{ return {Math::Degree::Tag(), static_cast<Math::Degree::value_type>(n)}; }

inline constexpr Math::Degree
operator "" _deg(unsigned long long int n)
{ return {Math::Degree::Tag(), static_cast<Math::Degree::value_type>(n)}; }

