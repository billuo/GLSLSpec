/**
 * @file Debug.hpp
 * @brief Macros only
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <cassert>
#include <Utility/Log.hpp>


#define TRACE(fmt, ...)     Log::t("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define INFO(fmt, ...)      Log::i("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG(fmt, ...)     Log::d("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define WARNING(fmt, ...)   Log::w("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define ERROR(fmt, ...)     Log::e("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define CRITICAL(fmt, ...)  Log::c("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define PRINT_VALUE(expr) DEBUG(#expr "={}", expr)
#define PRINT_TYPE(expr) DEBUG("typeof" #expr "={}", type_name<decltype(expr)>())

#if defined(DEBUG_BUILD)
#define UNREACHABLE assert(false)
#else
#if defined(__GNUC__)
#define UNREACHABLE __builtin_unreachable()
#else
#define UNREACHABLE abort();
#endif
#endif

/// Evaluate expr only ONCE.
#define ONCE(expr)                  \
    do {                            \
        static bool _done_ = false; \
        if (!_done_) {              \
            expr;                   \
            _done_ = true;          \
        }                           \
    } while (0)

#define UNUSED(x) (void(x))

