#include <Utility/Log.hpp>
#include <Options.hpp>
#include <Console.hpp>
#include <spdlog/sinks/null_sink.h>

#include <iostream>


Log::Log()
{
    try {
        Log::logger = std::make_shared<spdlog::logger>("logger", std::make_shared<spdlog::sinks::null_sink_mt>());
        register_logger(this->logger);
    } catch (spdlog::spdlog_ex& e) {
        std::cerr << "Logging initialization failed: " << e.what();
        std::cerr << std::endl;
    }
}

void
Log::add_sink(spdlog::sink_ptr ptr)
{ Instance.logger->sinks().push_back(ptr); }

Log Log::Instance;
