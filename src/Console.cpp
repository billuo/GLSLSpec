/**
 * @File Console.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Console.hpp>
#include <Window.hpp>
#include <Math/Math.hpp>
#include <OpenGL/Common.hpp>

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
{
    assert(!this->name.empty());
}

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

#if 0
void
declare_commands()
{
    Console::add_command({"help"}, {0, 1}, {"command"},
                         "Print help for command(s)",
                         [](const std::string& cmd, const std::string& args)
                         {
                             static const std::string header = options.application.name +
                                                               options.application.version +
                                                               options.application.acknowledgement;;
                             std::stringstream ss;
                             static const std::string prompt = "// ";
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 1) {
                                 ss << prompt << header << std::endl;
                                 ss << prompt << std::endl;
                                 for (auto& command : Console::Commands) {
                                     ss << prompt << command.description << std::endl;
                                 }
                             } else if (values.size() == 2) {
                                 for (auto& command : Console::Commands) {
                                     if (command.name == values[1]) {
                                         ss << prompt << command.description << std::endl;
                                         break;
                                     }
                                 }
                             } else {
                                 return false;
                             }
                             console->putline(ss.str());
                             return true;
                         });

    Console::add_command("version",
                         "version                return glslViewer version.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "version") {
                                 console->putline(options.application.version);
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("debug",
                         "debug[,on|off]         show/hide debug elements",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "debug") {
                                 console->putline(options.flags.debug_draw ? "on" : "off");
                                 return true;
                             } else {
                                 std::vector<std::string> values = split(args, ',');
                                 if (values.size() == 2) {
                                     std::lock_guard guard(console->mutex);
                                     options.flags.debug_draw = (values[1] == "on");
                                     return true;
                                 }
                             }
                             return false;
                         });

    Console::add_command("cursor",
                         "cursor[,on|off]        show/hide cursor",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "cursor") {
                                 std::string rta = options.window.cursor ? "on" : "off";
                                 std::cout << rta << std::endl;
                                 return true;
                             } else {
                                 std::vector<std::string> values = split(args, ',');
                                 if (values.size() == 2) {
                                     std::lock_guard guard(console->mutex);
                                     options.window.cursor = (values[1] == "on");
                                 }
                             }
                             return false;
                         });

    Console::add_command("window_height",
                         "window_height          return the height of the windows.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "window_height") {
                                 std::cout << main_window->dimension().y << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("pixel_density",
                         "pixel_density          return the pixel density.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "pixel_density") {
                                 std::cout << main_window-><< std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("screen_size",
                         "screen_size            return the screen size.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "screen_size") {
                                 glm::ivec2 screen_size = getScreenSize();
                                 std::cout << screen_size.x << ',' << screen_size.y << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("viewport",
                         "viewport               return the viewport size.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "viewport") {
                                 glm::ivec4 viewport = getViewport();
                                 std::cout << viewport.x << ',' << viewport.y << ',' << viewport.z << ','
                                           << viewport.w << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("mouse",
                         "mouse                  return the mouse position.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "mouse") {
                                 glm::vec2 pos = getMousePosition();
                                 std::cout << pos.x << "," << pos.y << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("fps",
                         "fps                    return u_fps, the number of frames per second.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "fps") {
                                 // Force the output in floats
                                 printf("%f\n", getFPS());
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("delta",
                         "delta                  return u_delta, the secs between frames.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "delta") {
                                 // Force the output in floats
                                 printf("%f\n", getDelta());
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("time",
                         "time                   return u_time, the elapsed time.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "time") {
                                 // Force the output in floats
                                 printf("%f\n", getTime());
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("date",
                         "date                   return u_date as YYYY, M, D and Secs.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "date") {
                                 // Force the output in floats
                                 glm::vec4 date = getDate();
                                 std::cout << date.x << ',' << date.y << ',' << date.z << ',' << date.w
                                           << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("culling",
                         "culling[,<disabled|front|back|both>]   get or set the culling modes",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 1) {
                                 std::cout << string_of_culling(sandbox->m_culling) << std::endl;
                                 return true;
                             } else if (values.size() == 2) {
                                 auto culling = culling_of_string(values[1]);
                                 if (culling == Culling::Bad) {
                                     Log::w("Unknown culling mode: {}. Culling mode unchanged.", values[1]);
                                 } else {
                                     sandbox->m_culling = culling_of_string(values[1]);
                                 }
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("frag",
                         "frag[,<filename>]      returns or save the fragment shader source code.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "frag") {
                                 std::cout << sandbox->shader_source(FRAGMENT) << std::endl;
                                 return true;
                             } else {
                                 std::vector<std::string> values = split(args, ',');
                                 if (values.size() == 2) {
                                     if (isDigit(values[1])) {
                                         // Line number
                                         unsigned int lineNumber = toInt(values[1]) - 1;
                                         std::vector<std::string>
                                                 lines = split(sandbox->shader_source(FRAGMENT), '\n', true);
                                         if (lineNumber < lines.size()) {
                                             std::cout << lineNumber + 1 << " " << lines[lineNumber] << std::endl;
                                         }

                                     } else {
                                         // Write shader into a file
                                         std::ofstream out(values[1]);
                                         out << sandbox->shader_source(FRAGMENT);
                                         out.close();
                                     }
                                     return true;
                                 } else if (values.size() > 2) {
                                     std::vector<std::string>
                                             lines = split(sandbox->shader_source(FRAGMENT), '\n', true);
                                     for (unsigned int i = 1; i < values.size(); i++) {
                                         unsigned int lineNumber = toInt(values[i]) - 1;
                                         if (lineNumber < lines.size()) {
                                             std::cout << lineNumber + 1 << " " << lines[lineNumber] << std::endl;
                                         }
                                     }
                                 }
                             }
                             return false;
                         });

    Console::add_command("vert",
                         "vert[,<filename>]      returns or save the vertex shader source code.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "vert") {
                                 std::cout << sandbox->shader_source(VERTEX) << std::endl;
                                 return true;
                             } else {
                                 std::vector<std::string> values = split(args, ',');
                                 if (values.size() == 2) {
                                     if (isDigit(values[1])) {
                                         // Line number
                                         unsigned int lineNumber = toInt(values[1]) - 1;
                                         std::vector<std::string>
                                                 lines = split(sandbox->shader_source(VERTEX), '\n', true);
                                         if (lineNumber < lines.size()) {
                                             std::cout << lineNumber + 1 << " " << lines[lineNumber] << std::endl;
                                         }

                                     } else {
                                         // Write shader into a file
                                         std::ofstream out(values[1]);
                                         out << sandbox->shader_source(VERTEX);
                                         out.close();
                                     }
                                     return true;
                                 } else if (values.size() > 2) {
                                     std::vector<std::string>
                                             lines = split(sandbox->shader_source(VERTEX), '\n', true);
                                     for (unsigned int i = 1; i < values.size(); i++) {
                                         unsigned int lineNumber = toInt(values[i]) - 1;
                                         if (lineNumber < lines.size()) {
                                             std::cout << lineNumber + 1 << " " << lines[lineNumber] << std::endl;
                                         }
                                     }
                                 }
                             }
                             return false;
                         });

    Console::add_command("dependencies",
                         "dependencies[,vert|frag]   returns all the dependencies of the vertex o fragment shader or both.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "dependencies") {
                                 auto& files = options.file.watch;
                                 for (auto& file : files) {
                                     if (file.type == FileType::GLSL_DEPENDENCY) {
                                         std::cout << file.path << std::endl;
                                     }
                                 }
                                 return true;
                             } else if (args == "dependencies,frag") {
                                 sandbox->printDependencies(FRAGMENT);
                                 return true;
                             } else if (args == "dependencies,vert") {
                                 sandbox->printDependencies(VERTEX);
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("files",
                         "files                  return a list of files.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "files") {
                                 size_t i = 0;
                                 for (auto&& file : options.file.watch) {
                                     std::cout << std::setw(2) << ++i << '.'
                                               << '[' << std::right << std::setw(12) << E<FileType>(file.type) << ']'
                                               << file.path << std::endl;
                                 }
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("buffers",
                         "buffers                return a list of buffers as their uniform name.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "buffers") {
                                 for (int i = 0; i < sandbox->getTotalBuffers(); i++) {
                                     std::cout << "u_buffer" << i << std::endl;
                                 }
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("defines",
                         "defines                return a list of active defines",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "defines") {
                                 for (const auto& define : options.defines) {
                                     std::cout << define << std::endl;
                                 }
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("define,",
                         "define,<KEYWORD>       add a define to the shader",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 2) {
                                 {
                                     std::lock_guard guard(console->mutex);
                                     options.define(values[1]);
                                 }
                                 std::lock_guard guard(options.file.mutex);
                                 options.force_update(sandbox->frag_index);
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("undefine,",
                         "undefine,<KEYWORD>     remove a define on the shader",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 2) {
                                 {
                                     std::lock_guard guard(console->mutex);
                                     options.undefine(values[1]);
                                 }
                                 std::lock_guard guard(options.file.mutex);
                                 options.force_update(sandbox->frag_index);
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("uniforms",
                         "uniforms[,all|active]  return a list of all uniforms and their values or just the one active (default).",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "uniforms,all") {
                                 // Print all Native Uniforms (they carry functions)
                                 for (auto& uniforms_function : sandbox->uniforms_functions) {
                                     std::cout << uniforms_function.second.type << ',' << uniforms_function.first;
                                     if (uniforms_function.second.print) {
                                         std::cout << "," << uniforms_function.second.print();
                                     }
                                     std::cout << std::endl;
                                 }
                             } else {
                                 // Print Native Uniforms (they carry functions) that are present on the shader
                                 for (auto& uniforms_function : sandbox->uniforms_functions) {
                                     if (uniforms_function.second.present) {
                                         std::cout << uniforms_function.second.type << ',' << uniforms_function.first;
                                         if (uniforms_function.second.print) {
                                             std::cout << "," << uniforms_function.second.print();
                                         }
                                         std::cout << std::endl;
                                     }
                                 }
                             }

                             // Print user defined uniform data
                             for (auto& it : sandbox->uniforms_data) {
                                 std::cout << it.second.getType() << "," << it.first;
                                 for (int i = 0; i < it.second.size; i++) {
                                     std::cout << ',' << it.second.value[i];
                                 }
                                 std::cout << std::endl;
                             }

                             for (int i = 0; i < sandbox->getTotalBuffers(); i++) {
                                 std::cout << "sampler2D," << "u_buffer" << i << std::endl;
                             }

                             for (auto& texture : sandbox->textures) {
                                 std::cout << "sampler2D," << texture.first << ',' << texture.second->getFilePath()
                                           << std::endl;
                             }

                             // TODO (not mine) - Cubemap

                             return true;
                         });

    Console::add_command("textures",
                         "textures               return a list of textures as their uniform name and path.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "textures") {
                                 for (auto& texture : sandbox->textures) {
                                     std::cout << texture.first << ',' << texture.second->getFilePath() << std::endl;
                                 }
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("window_width",
                         "window_width                   return the width of the windows.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "window_width") {
                                 std::cout << getWindowWidth() << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("camera_distance",
                         "camera_distance[,<dist>]       get or set the camera distance to the target.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 2) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getCamera().setDistance(toFloat(values[1]));
                                 sandbox->flagChange();
                                 return true;
                             } else {
                                 std::cout << sandbox->getCamera().getDistance() << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("camera_fov",
                         "camera_fov[,<field_of_view>]   get or set the camera field of view.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 2) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getCamera().setFOV(toFloat(values[1]));
                                 sandbox->flagChange();
                                 return true;
                             } else {
                                 std::cout << sandbox->getCamera().getFOV() << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("camera_position",
                         "camera_position[,<x>,<y>,<z>]  get or set the camera position.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 4) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getCamera()
                                        .setPosition(glm::vec3(toFloat(values[1]),
                                                               toFloat(values[2]),
                                                               toFloat(values[3])));
                                 sandbox->getCamera().lookAt(sandbox->getCamera().getTarget());
                                 sandbox->flagChange();
                                 return true;
                             } else {
                                 glm::vec3 pos = sandbox->getCamera().getPosition();
                                 std::cout << pos.x << ',' << pos.y << ',' << pos.z << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("camera_exposure",
                         "camera_exposure[,<aperture>,<shutterSpeed>,<sensitivity>]  get or set the camera exposure. Defaults: 16, 1/125s, 100 ISO",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 4) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getCamera()
                                        .setExposure(toFloat(values[1]), toFloat(values[2]), toFloat(values[3]));
                                 sandbox->flagChange();
                             } else {
                                 std::cout << sandbox->getCamera().getExposure() << std::endl;
                             }
                             return true;
                         });

    Console::add_command("light_position",
                         "light_position[,<x>,<y>,<z>]   get or set the light position.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 4) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getLight()
                                        .setPosition(glm::vec3(toFloat(values[1]),
                                                               toFloat(values[2]),
                                                               toFloat(values[3])));
                                 sandbox->getLight().bChange = true;
                                 sandbox->flagChange();
                                 return true;
                             } else {
                                 glm::vec3 pos = sandbox->getLight().getPosition();
                                 std::cout << pos.x << ',' << pos.y << ',' << pos.z << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("light_color",
                         "light_color[,<r>,<g>,<b>]      get or set the light color.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 4) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getLight().color =
                                         glm::vec3(toFloat(values[1]), toFloat(values[2]), toFloat(values[3]));
                                 sandbox->flagChange();
                                 return true;
                             } else {
                                 glm::vec3 color = sandbox->getLight().color;
                                 std::cout << color.x << ',' << color.y << ',' << color.z << std::endl;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("model_position",
                         "model_position[,<x>,<y>,<z>]   get or set the model position.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             std::vector<std::string> values = split(args, ',');
                             if (values.size() == 4) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->getModel()
                                        .setPosition(glm::vec3(toFloat(values[1]),
                                                               toFloat(values[2]),
                                                               toFloat(values[3])));
                                 sandbox->getLight().bChange = true;
                                 sandbox->flagChange();
                             } else {
                                 glm::vec3 pos = sandbox->getModel().getPosition();
                                 std::cout << pos.x << ',' << pos.y << ',' << pos.z << std::endl;
                             }
                             return true;
                         });

    Console::add_command("screenshot",
                         "screenshot[,<filename>]        saves a screenshot to a filename.",
                         [](const std::string& cmd, const std::string& args)
                         {
                             const auto output = options.file.output;
                             if (args == "screenshot" && !output.empty()) {
                                 std::lock_guard guard(console->mutex);
                                 sandbox->screenshotFile = output;
                                 return true;
                             } else {
                                 std::vector<std::string> values = split(args, ',');
                                 if (values.size() == 2) {
                                     std::lock_guard guard(console->mutex);
                                     sandbox->screenshotFile = values[1];
                                     return true;
                                 }
                             }
                             return false;
                         });

    Console::add_command("q",
                         "q                          close glslViewer",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "q") {
                                 options.flags.running = false;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("quit",
                         "quit                       close glslViewer",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "quit") {
                                 options.flags.running = false;
                                 return true;
                             }
                             return false;
                         });

    Console::add_command("exit",
                         "exit                       close glslViewer",
                         [](const std::string& cmd, const std::string& args)
                         {
                             if (args == "exit") {
                                 options.flags.running = false;
                                 return true;
                             }
                             return false;
                         });
}
#else

void
declare_commands()
{
    using Arguments = Command::Arguments;
    Console::add_command("help", {0, 1}, {"command"},
                         "Print help for command(s)",
                         [](const std::string& cmd, const Arguments& args)
                         {
                             static const std::string header = options.application.name + ' ' +
                                                               options.application.version +
                                                               options.application.acknowledgement;
                             switch (args.size()) {
                                 case 0:
                                     *console << header << "\n\n";
                                     for (auto& command : Console::Commands()) {
                                         *console << command.second.usage() << '\n';
                                     }
                                     break;
                                 case 1: {
                                     auto it = Console::Commands().find(cmd);
                                     if (it != Console::Commands().end()) {
                                         *console << it->second.usage() << '\n';
                                     }
                                 }
                                     break;
                                 default:
                                     return false;
                             }
                             return true;
                         });

    Console::add_command("version", {0, 0}, {},
                         "Display version string of application",
                         [](const std::string& cmd, const Arguments& args)
                         {
                             auto& glv = options.opengl.version;
                             *console << options.application.name << ' ' << options.application.version << '\n';
                             *console << "OpenGL " << glv.major << '.' << glv.minor << '\n';
                             *console << "glfw:" << glfwGetVersionString() << '\n';
                             return true;
                         });

    Console::add_command("debug", {0, 1}, {"on/off"},
                         "Display debug drawing status or enable/disable it.",
                         [](const std::string& cmd, const Arguments& args)
                         {
                             switch (args.size()) {
                                 case 0:
                                     *console << (options.flags.debug_draw ? "on" : "off") << '\n';
                                     return true;
                                 case 1: {
                                     const std::string& arg = args.front();
                                     if (arg == "on") {
                                         options.flags.debug_draw = true;
                                     } else if (arg == "off") {
                                         options.flags.debug_draw = false;
                                     } else {
                                         Log::i("Unknown argument: {}", arg);
                                     }
                                 }
                                     return true;
                                 default:
                                     Log::i("Excessive argument: {}", *++args.begin());
                                     return false;
                             }
                         });

    Console::add_command("viewport", {0, 0}, {},
                         "Display position and dimension of viewport.",
                         [](const std::string& cmd, const Arguments& args)
                         {
                             switch (args.size()) {
                                 case 0:
                                     *console << main_window->viewport() << '\n';
                                     return true;
                                 default:
                                     Log::i("Excessive arguments: {}", args.front());
                                     return false;
                             }
                         });
    // Console::add_command("command", {0, 0}, {},
    //                      "description",
    //                      [](const std::string& cmd, const Arguments& args)
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

#endif

void
Console::add_command(std::string name, std::pair<unsigned, unsigned> n_args,
                     std::initializer_list<std::string> arg_names, std::string description, Command::Action action)
{
    CommandTable.emplace(std::make_pair(name,
                                        Command(name, std::move(n_args), arg_names, std::move(description), action)));
}

bool
Console::execute(std::string line)
{
    if (line.front() == line.back()) {
        if (line.front() == '\'' || line.front() == '"') {
            line = line.substr(1, line.size() - 1);
        }
    }
    std::regex words("[\\w]+");
    auto begin_word = std::sregex_iterator(line.begin(), line.end(), words);
    auto end_word = std::sregex_iterator();
    auto&& cmd = begin_word->str();
    std::list<std::string> args;
    while (++begin_word != end_word) {
        args.emplace_back(std::move(begin_word->str()));
    }
    auto it = CommandTable.find(cmd);
    if (it == CommandTable.end() || args.size() < it->second.n_args.first || args.size() > it->second.n_args.second) {
        return false;
    }
    return it->second.action(cmd, args);
}

void
Console::execute_all(CommandQueue& commands)
{
    while (!commands.empty()) {
        std::string&& command = std::move(commands.front());
        commands.pop();
        if (!execute(command)) {
            Log::i("Can not find command: {}", command);
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
            Log::i("Can not find command: {}", line);
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

