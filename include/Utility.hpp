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

/// Return size of static array at compile time
#define countof(Array) (sizeof(Array) / sizeof(Array[0]))

template <class T, class O>
static inline T* pointer_offset(T* ptr, O offset) {
    return (T*)((char*)(ptr) + offset);
}

#endif /* end of include guard: UTILITY_HPP_ZUDUHVTJ */
