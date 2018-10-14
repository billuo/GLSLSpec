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

/// Print format string to stderr
#define DEBUG(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

/// Convert OpenGL error code to error string.
const char* GLErrorString(int error);

/// Perform OpenGL error checking with given logging verbosity.
#define CHECK_OPENGL()                                                                                         \
    do {                                                                                                       \
        if (GLint err = glGetError()) {                                                                        \
            DEBUG("Function:%s\nLine:%d\nOpenGL Error(%d):%s\n", __func__, __LINE__, err, GLErrorString(err)); \
            exit(err);                                                                                         \
        }                                                                                                      \
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
