#ifndef OPENGL_LAB_LOG_HPP
#define OPENGL_LAB_LOG_HPP
#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


class Log {

    static Log Instance;

    Log();
    void init();

    std::shared_ptr<spdlog::logger> console;

  public:

/// verbose(trace) logging
    template <typename ...Args>
    static void v(const char* fmt, Args&& ...args)
    { Log::Instance.console->trace(fmt, std::forward<Args>(args)...); }

/// debug logging
    template <typename ...Args>
    static void d(const char* fmt, Args&& ...args)
    { Log::Instance.console->debug(fmt, std::forward<Args>(args)...); }

/// info logging
    template <typename ...Args>
    static void i(const char* fmt, Args&& ...args)
    { Log::Instance.console->info(fmt, std::forward<Args>(args)...); }

/// warning logging
    template <typename ...Args>
    static void w(const char* fmt, Args&& ...args)
    { Log::Instance.console->warn(fmt, std::forward<Args>(args)...); }

/// error logging
    template <typename ...Args>
    static void e(const char* fmt, Args&& ...args)
    { Log::Instance.console->error(fmt, std::forward<Args>(args)...); }

/// critical logging
    template <typename ...Args>
    static void c(const char* fmt, Args&& ...args)
    { Log::Instance.console->critical(fmt, std::forward<Args>(args)...); }

};

#endif //OPENGL_LAB_LOG_HPP
