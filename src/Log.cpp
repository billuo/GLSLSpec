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
    auto file_sink = std::make_shared<sinks::basic_file_sink_mt>("log.txt");
    file_sink->set_level(level::trace);
    file_sink->set_pattern("[Thread %t] [%c] [%^%l%$]\n\t[%n] %v");
    auto console_sink = std::make_shared<sinks::stdout_color_sink_mt>();
    console_sink->set_level(console_sink_level);
    console_sink->set_pattern("[%T.%e] [%^%l%$] [%n] %v");
    sinks_init_list sinks = {console_sink, file_sink};
    console = std::make_shared<logger>("console", sinks);
    register_logger(console);
}

Log Log::Instance;
