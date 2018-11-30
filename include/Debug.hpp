/**
 * @file Debug.hpp
 * @brief Debug facilities including assertion and logging.
 * @author Zhen Luo 461652354@qq.com
 */
#ifndef DEBUG_HPP_W8W5TH9A
#define DEBUG_HPP_W8W5TH9A
#pragma once

#include "Utility.hpp"
#include <cassert>
#include <cstdio>


#define PRINT_TO_STDERR(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

#define ERROR(fmt, ...) PRINT_TO_STDERR(fmt, ##__VA_ARGS__)

/// Print format string to stderr
#if defined(DEBUG_BUILD)
#define DEBUG(fmt, ...) PRINT_TO_STDERR(fmt, ##__VA_ARGS__)
#else
#define DEBUG(...)
#endif

/// Convert OpenGL error code to error string.
const char* GLErrorString(uint32_t error);

/// Perform OpenGL error checking with given logging verbosity.
#define CHECK_OPENGL()                                                                                       \
    do {                                                                                                     \
        if (GLenum err = glGetError()) {                                                                      \
            ERROR("Function:%s\nLine:%d\nOpenGL Error(%d):%s", __func__, __LINE__, err, GLErrorString(err)); \
            exit(err);                                                                                       \
        }                                                                                                    \
    } while (0)

/// Evaluate expr only ONCE.
#define ONCE(expr)                  \
    do {                            \
        static bool _done_ = false; \
        if (!_done_) {              \
            expr;                   \
            _done_ = true;          \
        }                           \
    } while (0)

#endif /* end of include guard: DEBUG_HPP_W8W5TH9A */
