#ifndef OPENGL_LAB_SHADERCOMPILER_HPP
#define OPENGL_LAB_SHADERCOMPILER_HPP

#include "../Constants.hpp"
#include "../Object/Shader.hpp"
#include "../Object/Program.hpp"
#include "../Object/ProgramPipeline.hpp"

#include <unordered_map>


namespace OpenGL {

struct ShaderSource {
    explicit ShaderSource(const std::string& file);

    explicit ShaderSource(GLenum type, const std::string& source)
            : type(type), source(source)
    {}

    bool operator==(const ShaderSource& rhs) const
    { return type == rhs.type && source == rhs.source; }

    bool operator!=(const ShaderSource& rhs) const
    { return !(rhs == *this); }

    GLenum type = GL_UNKNOWN_SHADER;
    std::string source;
};

} // namespace OpenGL

namespace std {
template <>
struct hash<OpenGL::ShaderSource> {
    using argument_type = OpenGL::ShaderSource;
    using result_type = std::size_t;

    result_type operator()(const argument_type& ss) const noexcept
    {
        return std::hash<std::string>{}(ss.source) ^
               std::hash<GLenum>{}(ss.type);
    }
};
}

namespace OpenGL {

/// Compile strings, files, etc. containing shader sources into OpenGL Shaders
class ShaderCompiler {
  public:
    static ShaderCompiler& Instance()
    {
        static ShaderCompiler singleton;
        return singleton;
    }

    ShaderCompiler() = default;

    shared_shader
    compile(const ShaderSource& source, bool force_compile = false);

  private:
    std::unordered_map<ShaderSource, shared_shader> m_cache;
};

} // namespace OpenGL

#endif //OPENGL_LAB_SHADERCOMPILER_HPP
