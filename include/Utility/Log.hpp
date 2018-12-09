/**
 * @File Log.hpp
 * @brief Logging interface
 * @details Uses spdlog as implementation
 * @sa https://github.com/gabime/spdlog/wiki/1.-QuickStart
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/ostream.h"

class Log {
  public:

    static void add_sink(spdlog::sink_ptr ptr);

    /// verbose(trace) logging
    template <typename ...Args>
    static void v(const char* fmt, Args&& ...args)
    { Log::Instance.logger->trace(fmt, std::forward<Args>(args)...); }

    /// debug logging
    template <typename ...Args>
    static void d(const char* fmt, Args&& ...args)
    { Log::Instance.logger->debug(fmt, std::forward<Args>(args)...); }

    /// info logging
    template <typename ...Args>
    static void i(const char* fmt, Args&& ...args)
    { Log::Instance.logger->info(fmt, std::forward<Args>(args)...); }

    /// warning logging
    template <typename ...Args>
    static void w(const char* fmt, Args&& ...args)
    { Log::Instance.logger->warn(fmt, std::forward<Args>(args)...); }

    /// error logging
    template <typename ...Args>
    static void e(const char* fmt, Args&& ...args)
    { Log::Instance.logger->error(fmt, std::forward<Args>(args)...); }

    /// critical logging
    template <typename ...Args>
    static void c(const char* fmt, Args&& ...args)
    { Log::Instance.logger->critical(fmt, std::forward<Args>(args)...); }

  private:
    Log();

    static Log Instance;

    std::shared_ptr<spdlog::logger> logger;

};
