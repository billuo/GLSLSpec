#include "Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream>


#if defined(DEBUG_BUILD)
constexpr auto console_sink_level = spdlog::level::trace;
#else
constexpr auto console_sink_level = spdlog::level::info;
#endif

Log::Log()
{
    try {
        init();
    } catch (spdlog::spdlog_ex& e) {
        std::cerr << "Logging initialization failed: " << e.what();
        std::cerr << std::endl;
    }
}

void
Log::init()
{
    using namespace spdlog;
    // console logs goes to both console(color) and log file
    auto file_sink = std::make_shared<sinks::basic_file_sink_mt>("log.txt");
    file_sink->set_level(level::trace);
    file_sink->set_pattern("[Thread %t] [%c] [%^%l%$]\n\t[%n] %v");
    auto console_sink = std::make_shared<sinks::ansicolor_stdout_sink_mt>();
    console_sink->set_level(console_sink_level);
    console_sink->set_pattern("[%T.%e] [%^%l%$] [%n] %v");
    sinks_init_list sinks = {console_sink, file_sink};
    console = std::make_shared<logger>("console", sinks);
    register_logger(console);
    // raw logs goes to stderr only
    auto raw_sink = std::make_shared<sinks::stderr_color_sink_mt>();
    raw_sink->set_pattern("%v");
    raw_err = std::make_shared<logger>("raw", raw_sink);
    register_logger(raw_err);
}

Log Log::Instance;
