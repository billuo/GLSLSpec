/**
 * @file Debug.hpp
 * @brief Debug facilities including assertion and logging.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <cassert>
#include "Log.hpp"


#define DEBUG(fmt, ...) Log::d("{}:{} in {}\n\t" fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define VALUE(expr) DEBUG(#expr "={}", expr)

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

