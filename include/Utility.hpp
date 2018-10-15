/**
 * @file Utility.hpp
 * @brief Commonly used functions and macros
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef UTILITY_HPP_ZUDUHVTJ
#define UTILITY_HPP_ZUDUHVTJ
#pragma once

#include <cmath>
#include <cstdlib>
#include <new>
#include <utility>
#include <string>

/// Return size of static array at compile time
#define countof(Array) (sizeof(Array) / sizeof(Array[0]))

template <class T, class O>
static inline T* pointer_offset(T* ptr, O offset) {
    return (T*)((char*)(ptr) + offset);
}

#if !CXX_MSVC
char* SafeDemangle(const char* mangled_name, char* output_buffer, size_t* length);
#endif

/// Convert types to name strings.
/// @author Howard Hinnant
/// @see https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
template <typename T>
const char* type_name() {
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

#endif /* end of include guard: UTILITY_HPP_ZUDUHVTJ */
