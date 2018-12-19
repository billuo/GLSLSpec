/**
 * @File Console.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Console.hpp>
#include <Window.hpp>
#include <Math/Math.hpp>
#include <OpenGL/Common.hpp>
#include <Sandbox.hpp>
#include <iostream>
#include <fstream>
#include <regex>


std::unique_ptr<Console> console;

Command::Command(std::string name, std::pair<unsigned, unsigned> n_args,
                 std::initializer_list<std::string> arg_names, std::string description, Command::Action action)
        : name(std::move(name)),
          arg_names(arg_names),
          description(std::move(description)),
          n_args(std::move(n_args)),
          action(action)
{ assert(!this->name.empty()); }

std::string
Command::usage() const
{
    std::string ret = name;
    unsigned nth_arg = 1;
    for (auto&& arg_name : arg_names) {
        ret += ' ';
        if (nth_arg <= n_args.first) {
            ret += '<';
            ret += arg_name;
            ret += '>';
        } else {
            ret += '[';
            ret += arg_name;
            ret += ']';
        }
        ++nth_arg;
    }
    if (n_args.second == -1u) {
        ret += "...";
    }
    ret += "\n\t" + description;
    return ret;
}

std::map<std::string, Command> Console::CommandTable;

Console::AsyncGetline::AsyncGetline()
{
    std::thread([&]()
                {
                    do {
                        std::string line;
                        while (true) {
                            while (options.flags.running && std::cin.peek() == EOF) {
                                std::this_thread::yield();
                            }
                            if (!options.flags.running) {
                                break;
                            }
                            int c = std::cin.get();
                            if (c == '\n') {
                                break;
                            }
                            line += static_cast<char>(c);
                        }
                        std::lock_guard guard(this->mutex);
                        m_lines.push(line);
                        console->flush(true);
                    } while (options.flags.running);
                }).detach();
}

void
declare_commands()
{
    using Arguments = Command::Arguments;
    Console::add_command("help", {0, 1}, {"command"},
                         "Print help for command(s)",
                         [](std::string cmd, Arguments args)
                         {
                             static const std::string header = options.application.name + ' ' +
                                                               options.application.version +
                                                               options.application.acknowledgement;
                             if (args.empty()) {
                                 *console << header << '\n';
                                 *console << "Format: command-name <argument> [optional argument]" << '\n';
                                 *console << "Optional argument followed by '...' "
                                             "can be a space separated list of arbitrary length." << "\n\n";
                                 for (auto& command : Console::Commands()) {
                                     *console << command.second.usage() << '\n';
                                 }
                             } else {
                                 assert(args.size() == 1);
                                 auto it = Console::Commands().find(cmd);
                                 if (it != Console::Commands().end()) {
                                     *console << it->second.usage() << '\n';
                                 }
                             }
                         });

    Console::add_command("version", {0, 0}, {},
                         "Display version string of application",
                         [](std::string cmd, Arguments args)
                         {
                             assert(args.empty());
                             auto& glv = options.opengl.version;
                             *console << options.application.name << ' ' << options.application.version << '\n';
                             *console << "OpenGL " << glv.major << '.' << glv.minor << '\n';
                             *console << "glfw:" << glfwGetVersionString() << '\n';
                         });

    Console::add_command("debug", {0, 1}, {"on|off"},
                         "Display debug drawing status or enable/disable it.",
                         [](std::string cmd, Arguments args)
                         {
                             if (args.empty()) {
                                 *console << (options.flags.debug_draw ? "on" : "off") << '\n';
                             } else {
                                 assert(args.size() == 1);
                                 const std::string& arg = args.front();
                                 if (arg == "on") {
                                     options.flags.debug_draw = true;
                                 } else if (arg == "off") {
                                     options.flags.debug_draw = false;
                                 } else {
                                     Log::i("{}: Unknown argument: {}", cmd, arg);
                                 }
                             };
                         });

    Console::add_command("viewport", {0, 0}, {},
                         "Display position and dimension of viewport.",
                         [](std::string cmd, Arguments args)
                         {
                             assert(args.empty());
                             *console << main_window->viewport() << '\n';
                         });
    Console::add_command("resize", {1, 2}, {"side|width", "height"},
                         "Resize main window",
                         [](std::string cmd, Arguments args)
                         {
                             if (!options.flags.resizable) {
                                 Log::e("Window resizing already disabled.");
                                 return;
                             }
                             switch (args.size()) {
                                 case 1: {
                                     int sz = string_to<int>(args.front());
                                     main_window->resize(sz, sz);
                                 }
                                     break;
                                 case 2: {
                                     auto it = args.begin();
                                     int w = string_to<int>(*it++);
                                     int h = string_to<int>(*it++);
                                     main_window->resize(w, h);
                                 }
                                     break;
                                 default:
                                     UNREACHABLE;
                             }
                         });
    Console::add_command("define", {0, -1u}, {"macro=value"},
                         "Show the list of defined macros, or define more. (Seen by shader preprocessor)",
                         [](std::string cmd, Arguments args)
                         {
                             if (args.empty()) {
                                 *console << "Macros defined:\n";
                                 for (auto& macro : options.defines) {
                                     *console << '\t' << macro << '\n';
                                 }
                             } else {
                                 for (auto&& arg : args) {
                                     options.define(arg);
                                 }
                                 sandbox->recompile();
                             }
                         });
    Console::add_command("mouse", {0, 0}, {},
                         "Display current mouse position",
                         [](std::string cmd, Arguments)
                         { *console << main_window->mouse_position() << '\n'; });
    Console::add_command("frame", {0, 0}, {},
                         "Display current frame buffer size in pixels",
                         [](std::string cmd, Arguments args)
                         { *console << main_window->frame_buffer_size() << '\n'; });
    Console::add_command("programs", {0, 0}, {},
                         "Display current imported shader programs by their OpenGL object names and optional labels.",
                         [](std::string cmd, Arguments args)
                         {
                             *console << "Program list:\n";
                             for (auto&&[name, label] : sandbox->programs()) {
                                 *console << "\tProgram" << name << ':' << label << '\n';
                             }
                         });
    Console::add_command("program", {1, 2}, {"name", "interface name:uniform|uniform_block|input"},
                         "Introspect specified program and optionally in the specified interface.",
                         [](std::string cmd, Arguments args)
                         {
                             auto name = string_to<GLuint>(args.front());
                             auto&& intro = OpenGL::Introspector::Get(name).lock();
                             if (!intro) {
                                 return;
                             }
                             *console << "Introspecting Program" << args.front() << ":\n";
                             if (args.size() == 2) {
                                 if (args.back() == "uniform") {
                                     *console << intro->uniform() << '\n';
                                 } else if (args.back() == "uniform_block") {
                                     *console << intro->uniform_block() << '\n';
                                 } else if (args.back() == "input") {
                                     *console << intro->input() << '\n';
                                 } else {
                                     Log::w("Unimplemented: print interface {} to console", args.back());
                                 }
                             } else {
                                 *console << *intro << '\n';
                             };
                         });
    // Console::add_command("command", {0, 0}, {},
    //                      "description",
    //                      [](std::string cmd, Arguments args)
    //                      {
    //                          switch(args.size()){
    //                              case 0:
    //                                  break;
    //                              default:
    //                                  return false;
    //                          }
    //                          return true;
    //                      });
}

void
Console::add_command(std::string name, std::pair<unsigned, unsigned> n_args,
                     std::initializer_list<std::string> arg_names, std::string description, Command::Action action)
{
    CommandTable.emplace(name, Command(name, std::move(n_args), arg_names, std::move(description), action));
}

bool
Console::execute(std::string line)
{
    // split line with space as separator
    std::regex spaces("\\s+");
    auto token = std::sregex_token_iterator(line.begin(), line.end(), spaces, -1);
    auto end = std::sregex_token_iterator();
    // first word is command name
    std::string cmd = *token++;
    if (cmd.empty()) { // when line begins with spaces
        cmd = *token++;
    }
    // early exit if command not found
    auto it = CommandTable.find(cmd);
    if (it == CommandTable.end()) {
        return false;
    }
    // the rest are arguments
    std::list<std::string> args;
    auto&&[min_args, max_args] = it->second.n_args;
    while (token != end && args.size() < max_args) {
        args.emplace_back(*token++);
    }
    // check number of arguments
    if (args.size() < min_args) {
        Log::e("{} needs at least {} arguments:\n{}", cmd, min_args, it->second.usage());
        return false;
    } else if (args.size() == max_args && token != end) {
        Log::e("Excessive arguments: {}\n{}", *token, it->second.usage());
        return false;
    }
    it->second.action(std::move(cmd), std::move(args));
    return true;
}

void
Console::execute_all(CommandQueue& commands)
{
    while (!commands.empty()) {
        std::string&& command = std::move(commands.front());
        commands.pop();
        if (!execute(command)) {
            Log::i("Can not execute command: {}", command);
        }
    }
    execute_all();
}

void
Console::execute_all()
{
    while (true) {
        std::string&& line = asyncGetline.getline();
        if (line.empty()) {
            break;
        }
        if (!execute(line)) {
            Log::i("Can not execute command: '{}'", line);
        }
    }
}

void
Console::ConsoleSink::flush_()
{
    std::string&& output = oss.str();
    oss.clear();
    oss.str(std::string());
    if (!output.empty()) {
        std::cout << output << options.application.prompt << std::flush;
    }
}

void
Console::ConsoleSink::sink_it_(const spdlog::details::log_msg& msg)
{
    fmt::memory_buffer formatted;
    sink::formatter_->format(msg, formatted);
    std::string&& str = fmt::to_string(formatted);
    oss << str;
}

