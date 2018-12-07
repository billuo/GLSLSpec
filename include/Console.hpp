#pragma once

#include "Options.hpp"

#include <list>
#include <string>
#include <queue>
#include <mutex>
#include <memory>


void
declare_commands();

struct Command {
    using Action = bool (*)(const std::string&, const std::string&);

    Command(std::initializer_list<std::string> names, std::pair<unsigned, unsigned> n_args,
            std::initializer_list<std::string> arg_names, std::string description, Action action);

    bool operator<(const Command& rhs) const
    { return names < rhs.names; }

    std::list<std::string> names;
    std::list<std::string> arg_names;
    std::string description;
    std::pair<unsigned, unsigned> n_args;
    Action action;
};

using CommandQueue = std::queue<std::string>;

class Console {
    struct AsyncGetline {
      public:
        explicit AsyncGetline(CommandQueue&& initial_commands);

        ~AsyncGetline() = default;

        std::string getline()
        {
            std::lock_guard guard(mutex);
            if (!m_commands.empty()) {
                auto ret = m_commands.front();
                m_commands.pop();
                return ret;
            }
            return {};
        }

      private:
        /// User input lines of commands.
        CommandQueue m_commands;
        /// Mutex guarding m_lines
        std::mutex mutex;
    };

  public:
    explicit Console(CommandQueue queue) : asyncGetline(std::move(queue))
    { put(options.application.prompt); }

    static void add_command(std::initializer_list<std::string> names, std::pair<unsigned, unsigned> n_args,
                            std::initializer_list<std::string> arg_names, std::string description,
                            Command::Action action);

    bool execute(const std::string& line);

    void execute_all();

    void put(const std::string& str);

    void putline(const std::string& str);

    std::mutex mutex; // Currently it seems to be locked only by one thread...

  private:
    AsyncGetline asyncGetline;
    static std::vector<Command> CommandTable;
};

extern std::unique_ptr<Console> console;


