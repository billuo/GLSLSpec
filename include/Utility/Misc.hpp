/**
 * @file Utility.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Utility/Log.hpp>

#include <cmath>
#include <cstdlib>
#include <new>
#include <memory>
#include <utility>
#include <string> ///< serialization
#include <ostream> ///< serialization


/// Return size of static array at compile time
template <typename T, std::size_t N>
inline constexpr auto
countof(T(& array)[N])
{ return N; }

template <typename T, typename D = std::default_delete<T>> using Owned = std::unique_ptr<T, D>;
template <typename T> using Shared = std::shared_ptr<T>;

template <typename E, typename = std::enable_if_t<std::is_enum_v<E>>>
inline constexpr auto
underlying_cast(E e)
{ return static_cast<std::underlying_type_t<std::decay_t<E>>>(e); }

#if !CXX_MSVC
char*
SafeDemangle(const char* mangled_name, char* output_buffer, size_t* length);
#endif

/// Convert types to name strings.
/// @author Howard Hinnant
/// @see https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
template <typename T>
inline const char*
type_name()
{
    using TR = typename std::remove_reference<T>::type;
    static std::string r;
    if (!r.empty()) {
        return r.c_str();
    }
#if !CXX_MSVC
    size_t length = 80;
    auto buffer = static_cast<char*>(malloc(length));
    buffer = SafeDemangle(typeid(TR).name(), buffer, &length);
    r = buffer;
    free(buffer);
#else
    r = typeid(TR).name();
#endif

    if (std::is_const<TR>::value) {
        r += " const";
    }
    if (std::is_volatile<TR>::value) {
        r += " volatile";
    }
    if (std::is_lvalue_reference<T>::value) {
        r += "&";
    } else if (std::is_rvalue_reference<T>::value) {
        r += "&&";
    }

    return r.c_str();
}

template <typename T>
inline T
string_to(const std::string& str);

template <>
inline int
string_to<int>(const std::string& str)
{
    try {
        return std::stoi(str);
    } catch (std::invalid_argument& e) {
        Log::e("{} can not be converted to int", str);
    } catch (std::out_of_range& e) {
        Log::e("{} out of range of int", str);
    }
    return 0;
}

template <>
inline long
string_to<long>(const std::string& str)
{
    try {
        return std::stol(str);
    } catch (std::invalid_argument& e) {
        Log::e("{} can not be converted to long", str);
    } catch (std::out_of_range& e) {
        Log::e("{} out of range of long, str");
    }
    return 0;
}

template <>
inline long long
string_to<long long>(const std::string& str)
{
    try {
        return std::stoll(str);
    } catch (std::invalid_argument& e) {
        Log::e("{} can not be converted to long long", str);
    } catch (std::out_of_range& e) {
        Log::e("{} out of range of long long", str);
    }
    return 0;
}

template <>
inline float
string_to<float>(const std::string& str)
{
    char* end;
    float ret = std::strtof(str.c_str(), &end);
    if (end == str.c_str()) {
        Log::e("{} can not be converted to float", str);
    } else if (errno == ERANGE) {
        Log::e("{} out of range of float", str.substr(0, end - str.c_str()));
    }
    return ret;
}

template <>
inline double
string_to<double>(const std::string& str)
{
    char* end;
    double ret = std::strtod(str.c_str(), &end);
    if (end == str.c_str()) {
        Log::e("{} can not be converted to float", str);
    } else if (errno == ERANGE) {
        Log::e("{} out of range of float", str.substr(0, end - str.c_str()));
    }
    return ret;
}
