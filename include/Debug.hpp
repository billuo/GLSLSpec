/**
 * @file Debug.hpp
 * @brief Debug facilities including assertion and logging.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef DEBUG_HPP_W8W5TH9A
#define DEBUG_HPP_W8W5TH9A
#pragma once

 #include <cassert>


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

#endif /* end of include guard: DEBUG_HPP_W8W5TH9A */
