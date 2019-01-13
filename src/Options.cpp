/**
 * @File Options.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Console.hpp>
#include <Utility/Misc.hpp>

#include <iostream>


GlobalOptions options;

GlobalOptions::GlobalOptions() :
        application({APP_NAME,
                     APP_VERSION,
                     DEBUG_BUILD ? "debug" : "release",
                     "\n***based on glslViewer 1.5.6 by Patricio Gonzalez Vivo(patriciogonzalezvivo.com)***"})
{}

namespace {

using Parser = unsigned (*)(const std::string& match, unsigned argc, const std::string* argv);

Parser AddShaderToWatch = [](const std::string& match, unsigned, const std::string* arg)
{
    options.input_files.emplace_back(*arg, FileType::Shader, "user");
    return 0u;
};

struct NamedOption {
    /// A string of single character names of option, when used should be specified as '-x' resp.
    const std::string short_names;
    /// An array of string names of option, when used should be specified as '--looong' resp.
    const std::vector<std::string> long_names;
    /// Description of this option, displayed in usage.
    const std::string description;
    /// Range of number of acceptable arguments {min, max}.
    const std::pair<unsigned, unsigned> n_args;
    /// Name of arguments
    const std::vector<std::string> arg_names;
    /// The arguments to this option are passed to the option specific parser to be made use of.
    const Parser parser;

    NamedOption(std::string shorts, std::initializer_list<std::string> longs, std::string description,
                std::pair<unsigned, unsigned> n_args, std::initializer_list<std::string> arg_names, Parser parser)
            : short_names(std::move(shorts)),
              long_names(longs),
              description(std::move(description)),
              n_args(std::move(n_args)),
              arg_names(arg_names),
              parser(parser)
    {}
};

// struct Importable {
//     const std::set<std::string> extensions;
//     const std::string description;
//     FileType type;
// };

//region Options

const std::vector<NamedOption> NamedOptions = {
        {"",  {"help"},
                "Display this help",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string*) -> unsigned
                {
                    options.flags.display_help = true;
                    return 0u;
                }},
        {"v", {"version"},
                "Print version of application",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string*) -> unsigned
                {
                    // Log::i("{} {} build v{}", PLATFORM_NAME, DEBUG_BUILD ? "debug" : "release", APP_VERSION);
                    *console << PLATFORM_NAME << ' ' << (DEBUG_BUILD ? "debug" : "release") << " v" << APP_VERSION
                             << '\n';
                    return 0u;
                }},
        {"t", {"top"},
                "Make the window always on top",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string*) -> unsigned
                {
                    options.window.always_on_top = true;
                    return 0u;
                }},
        {"x", {},
                "Set the X position of the billboard on the screen",
                {1, 1}, {"pixels"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.window.position.x = std::stoi(*arg);
                    return 1u;
                }},
        {"y", {},
                "Set the Y position of the billboard on the screen",
                {1, 1}, {"pixels"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.window.position.y = string_to<int>(*arg);
                    return 1u;
                }},
        {"w", {},
                "Set the width of the billboard on the screen",
                {1, 1}, {"pixels"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.window.dimension.x = string_to<int>(*arg);
                    return 1u;
                }},
        {"h", {},
                "Set the height of the billboard on the screen",
                {1, 1}, {"pixels"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.window.dimension.y = string_to<int>(*arg);
                    return 1u;
                }},
        {"",  {"fixed-size"},
                "Disable resizing window",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.flags.resizable = false;
                    return 0u;
                }},
        {"",  {"ttl"},
                "Exit the application after this many seconds. (can be floating point number)",
                {1, 1}, {"seconds"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.application.TTL = 1000 * static_cast<uint64_t>(string_to<float>(*arg));
                    Log::i("Application will quit after {}ms.", options.application.TTL);
                    return 1u;
                }},
        {"o", {"out",  "output"},
                "Save a screen shot before exiting to the specified file",
                {1, 1}, {"file"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    // TODO check extension
                    options.output_files.emplace_back(*arg);
                    return 1u;
                }},
        {"",  {"hidden"},
                "Don not display the window after startup",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string*) -> unsigned
                {
                    options.window.hidden = true;
                    return 0u;
                }},
        {"I", {},
                "An additional include directory for shader to #include from (NOTE: a space as separator is necessary)",
                {1, 1}, {"directories..."},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    auto&& ex_path = FS::canonical(*arg);
                    if (ex_path) {
                        options.includes.emplace_back(*ex_path);
                        return 1u;
                    } else {
                        Log::e("{}", ex_path.error());
                        return 0u;
                    }
                }},
        {"D", {},
                "An additional #define directive for shader",
                {1, 1}, {"name"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.define(*arg);
                    return 1u;
                }},
        {"e", {},
                "Execute a command when start. Can specify multiple ones and they will be carried out in order",
                {1, 1}, {"command"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    Log::e("Execute commands through command line will be removed");
                    options.initial_commands.emplace(*arg);
                    return 1u;
                }},
        {"E", {},
                "Same as -e, but demand the application to quit after finishing all commands specified through options.",
                {1, 1}, {"command"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    Log::e("Execute commands through command line will be removed");
                    options.flags.exit_after_execution = true;
                    options.initial_commands.emplace(*arg);
                    return 1u;
                }},
        {"",  {"debug"},
                "Enable debug drawing",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.flags.debug_draw = true;
                    return 0u;
                }},
        {"",  {"verbose"},
                "Enable verbose output",
                {0, 0}, {},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.flags.verbose = true;
                    return 0u;
                }},
        {"",  {"background"},
                "Load a fragment shader as background rendering shader",
                {1, 1}, {"shader.frag"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.input_files.emplace_back(*arg, FileType::Shader, "background");
                    return 1u;
                }},
        {"",  {"postprocess"},
                "Load a fragment shader as postprocessing shader",
                {1, 1}, {"shader.frag"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.input_files.emplace_back(*arg, FileType::Shader, "postprocess");
                    return 1u;
                }},
        {"C", {},
                "Load and draw an environmental cube map",
                {1, 1}, {"cubemap.png/jpg/hdr"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.input_files.emplace_back(*arg, FileType::Image, "cubemap");
                    return 1u;
                }},
        {"S", {},
                "Load and draw an environmental sphere map",
                {1, 1}, {"spheremap.png/jpg/hdr"},
                [](const std::string&, unsigned, const std::string* arg) -> unsigned
                {
                    options.input_files.emplace_back(*arg, FileType::Image, "spheremap");
                    return 1u;
                }},
        // Below are special options with a short name of '.',
        // which means these arguments are parsed as paths and matched by file extension (in long_name).
        // More importantly, the parser now only receive the option name, or file name itself. n_args is ignored.
        // XXX These special cases must be the last in the Options vector! It's assumed in parse_options().
        // XXX Also, none of the normal options shall have a short name containing '.' to avoid ambiguity.
        {".", {"vert", "vs"},
                "Load vertex shader",
                {0, 0}, {"shader"},
                AddShaderToWatch},
        {".", {"tesc"},
                "Load tessellation control shader",
                {0, 0}, {"shader"},
                AddShaderToWatch},
        {".", {"tese"},
                "Load tessellation evaluation shader",
                {0, 0}, {"shader"},
                AddShaderToWatch},
        {".", {"geom"},
                "Load geometry shader",
                {0, 0}, {"shader"},
                AddShaderToWatch},
        {".", {"frag", "fs"},
                "Load fragment shader",
                {0, 0}, {"shader"},
                AddShaderToWatch},
        {".", {"ply",  "obj", "OBJ", "PLY"},
                "Load an .obj or .ply file",
                {0, 0}, {"model"},
                [](const std::string& match, unsigned, const std::string* arg) -> unsigned
                {
                    options.input_files.emplace_back(*arg, FileType::Geometry, "geometry");
                    return 0u;
                }},
        {".", {"hdr",  "png", "jpg", "jpeg", "HDR", "PNG", "JPG", "JPEG"},
                "Load and assign a texture",
                {0, 0}, {"image"},
                [](const std::string& match, unsigned, const std::string* arg) -> unsigned
                {
                    options.input_files.emplace_back(*arg, FileType::Image, "texture");
                    return 0u;
                }}
};

//endregion

template <typename Pred>
auto
find_in_options(Pred&& P)
{ return std::find_if(NamedOptions.begin(), NamedOptions.end(), std::forward<Pred>(P)); }

template <typename Container>
bool
contains(const Container& c, typename Container::value_type const& v)
{ return std::find(c.begin(), c.end(), v) != c.end(); }

auto
first_extension_option()
{
    auto ret = std::find_if(NamedOptions.begin(), NamedOptions.end(), [](const NamedOption& opt)
    { return opt.short_names == "."; });
    return ret;
}

} // namespace

void
print_usage(const std::string& name)
{
    auto print_line = [](const std::string& str)
    { std::cout << str << '\n'; };
    print_line("Usage: " + name + " <arguments>");
    print_line("Arguments:");
    for (auto&& opt : NamedOptions) {
        std::string line = "\t";
        if (opt.short_names != ".") {
            for (auto c : opt.short_names) {
                line += '-';
                line += c;
                line += ' ';
            }
            for (auto&& n : opt.long_names) {
                line += "--" + n + ' ';
            }
            for (auto&& n : opt.arg_names) {
                line += "<" + n + "> ";
            }
            line.pop_back();
        } else {
            line += opt.arg_names[0] + '.';
            line += '[';
            for (auto&& n : opt.long_names) { // seen as list of extension names
                line += n + '/';
            }
            line.pop_back();
            line += ']';
        }
        print_line(line);
        print_line("\t\t" + opt.description);
    }
}

void
parse_options(int argc, char* argv[])
{
    static const auto ext_opt = first_extension_option(); // XXX
    std::vector<std::string> arguments;
    for (int i = 0; i < argc; ++i) {
        arguments.emplace_back(argv[i]);
    }
    for (auto current = arguments.cbegin(); current != arguments.cend(); ++current) {
        const auto& arg = *current;
        std::string match;
        if (arg[0] == '-') {
            // match as options
            decltype(NamedOptions)::const_iterator opt;
            if (arg[1] == '-') {
                // match in long name
                opt = find_in_options([arg](const NamedOption& option)
                                      { return contains(option.long_names, arg.substr(2)); });
                match = arg.substr(2);
            } else {
                // match in short name
                if (arg.size() != 2 || arg[1] == '.') {
                    throw invalid_option("Invalid short option: " + arg);
                }
                opt = find_in_options([arg](const NamedOption& option)
                                      { return contains(option.short_names, arg[1]); });
                match = arg[1];
            }
            if (opt == NamedOptions.end()) {
                throw invalid_option("Unknown option: " + arg);
            }
            // extract arguments for this option
            auto it = current + 1;
            while (it != arguments.end() && it <= current + opt->n_args.second) {
                if (it->at(0) == '-') {
                    break;
                }
                ++it;
            }
            --it;
            if (it - current < opt->n_args.first) {
                throw invalid_option("Insufficient amount of argument to option: " + arg);
            }
            current += opt->parser(match, static_cast<unsigned>(it - current), &*it);
        } else {
            // match by file extension
            auto&& ext = FS::path(arg).extension().string();
            if (ext.empty()) {
                continue;
            }
            ext = ext.substr(1);
            for (auto it = ext_opt; it != NamedOptions.end(); ++it) {
                if (contains(it->long_names, ext)) {
                    current += it->parser(ext, 1, &arg);
                    break;
                }
            }
        }
    }
}

