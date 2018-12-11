#pragma once

#include "Options.hpp"
#include "spdlog/sinks/base_sink.h"

#include <sstream>
#include <iomanip>
#include <list>
#include <string>
#include <queue>
#include <mutex>
#include <memory>
#include <map>


void
declare_commands();

struct Command {
    using Action = bool (*)(const std::string&, const std::list<std::string>&);

    Command(std::string name, std::pair<unsigned, unsigned> n_args,
            std::initializer_list<std::string> arg_names, std::string description, Action action);

    bool operator<(const Command& rhs) const
    { return name < rhs.name; }

    bool operator==(const Command& rhs) const
    { return name == rhs.name; }

    bool operator!=(const Command& rhs) const
    { return !(rhs == *this); }

    std::string usage() const;

    std::string name;
    std::list<std::string> arg_names;
    std::string description;
    std::pair<unsigned, unsigned> n_args;
    Action action;
};

using CommandQueue = std::queue<std::string>;

/// Console serves as an IO interface between user and application.
class Console {
  public:
    explicit Console() : asyncGetline(), m_sink(std::make_shared<ConsoleSink>())
    {
        Log::add_sink(m_sink);
        m_sink->set_pattern("==>[%^%l%$] %v");
        m_sink->oss << std::fixed;
    }

    static void add_command(std::string names, std::pair<unsigned, unsigned> n_args,
                            std::initializer_list<std::string> arg_names, std::string description,
                            Command::Action action);

    static const auto& Commands()
    { return CommandTable; }

    /// @brief Execute a single line of command.
    /// @param [in] line The command to execute.
    /// @return If the execution was successful.
    bool execute(std::string line);

    /// @brief Execute all given commands AND commands from input.
    /// @param [in,out] commands Externally given commands. Executed before commands from user input.
    void execute_all(CommandQueue& commands);

    /// @brief Execute all commands from input.
    void execute_all();

    /// @brief Flush the console output stream to display any messages still in buffer.
    /// @param force If true, even when no output is available, print a newline together with the prompt.
    void flush(bool force = false)
    { return m_sink->flush(); }

    template <typename T>
    Console& operator<<(T&& x)
    {
        m_sink->oss << x;
        return *this;
    }

  private:

    struct AsyncGetline {
      public:
        explicit AsyncGetline();

        ~AsyncGetline() = default;

        std::string getline()
        {
            std::lock_guard guard(mutex);
            if (!m_lines.empty()) {
                auto ret = m_lines.front();
                m_lines.pop();
                return ret;
            }
            return {};
        }

      private:
        /// User input lines of commands.
        CommandQueue m_lines;
        /// Mutex guarding m_lines
        std::mutex mutex;
    };

    class ConsoleSink : public spdlog::sinks::base_sink<std::mutex> {
      public:
        std::ostringstream oss;
      protected:
        void sink_it_(const spdlog::details::log_msg& msg) override;
        void flush_() override;
    };

    static std::map<std::string, Command> CommandTable;

    AsyncGetline asyncGetline;

    std::shared_ptr<ConsoleSink> m_sink;

    friend class Log;
};

extern std::unique_ptr<Console> console;


