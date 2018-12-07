#include "Log.hpp"
#include "Options.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <iostream>


#if DEBUG_BUILD
constexpr auto default_level = spdlog::level::debug;
#else
constexpr auto default_level = spdlog::level::info;
#endif

Log::Log()
{
    try {
        initialize();
    } catch (spdlog::spdlog_ex& e) {
        std::cerr << "Logging initialization failed: " << e.what();
        std::cerr << std::endl;
    }
}

void
Log::initialize()
{
    using namespace spdlog;
    set_level(default_level);
    auto stdout_mt = std::make_shared<sinks::ansicolor_stdout_sink_mt>();
    stdout_mt->set_pattern("==>[%^%l%$] %v");
    sinks_init_list sinks = {stdout_mt};
    Log::logger = std::make_shared<spdlog::logger>("logger", sinks);
    register_logger(this->logger);
}

Log Log::Instance;
