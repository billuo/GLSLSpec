#include <cstring>
#include <fstream>
#include <map>
#include <vector>

#include "OpenGL/Shader.hpp"

namespace OpenGL {

static const char* ShaderTypeString(GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER: return "vertex shader";
    case GL_TESS_CONTROL_SHADER: return "tessellation control shader";
    case GL_TESS_EVALUATION_SHADER: return "tessellation evaluation shader";
    case GL_GEOMETRY_SHADER: return "geometry shader";
    case GL_FRAGMENT_SHADER: return "fragment shader";
    case GL_COMPUTE_SHADER: return "compute shader";
    default: return "UNKNOWN";
    }
}

void Shader::Source(const GLchar** sources, size_t count) {
    if (aux_CheckInitialized(true)) {
        std::vector<GLint> lens(count);
        for (size_t i = 0; i < count; ++i) {
            lens[i] = strlen(sources[i]);
        }
        glShaderSource(Name(), count, sources, lens.data());
    }
}

void Shader::Compile() {
    if (aux_CheckInitialized(true)) {
        glCompileShader(Name());
        if (aux_GetParameter(GL_COMPILE_STATUS) == GL_FALSE) {
            DEBUG("%s compilation failed:%s", ShaderTypeString(aux_GetParameter(GL_SHADER_TYPE)),
                  aux_GetInfoLog().get());
        }
    }
}

GLint Shader::aux_GetParameter(GLenum pname) const {
    GLint result = -1;
    if (aux_CheckInitialized(true)) {
        glGetShaderiv(Name(), pname, &result);
    }
    return result;
}

std::unique_ptr<char[]> Shader::aux_GetInfoLog() const {
    std::unique_ptr<char[]> ret;
    if (aux_CheckInitialized(true)) {
        GLint length = aux_GetParameter(GL_INFO_LOG_LENGTH);
        assert(length >= 0);
        ret = std::make_unique<char[]>(length + 1); // XXX one more byte, just in case
        glGetShaderInfoLog(Name(), length, nullptr, ret.get());
    }
    return ret;
}

namespace ShaderResource {

static GLenum SuffixType(std::string suffix) {
    static const auto map = std::map<std::string, GLenum>{
        { "vert", GL_VERTEX_SHADER },   { "tesc", GL_TESS_CONTROL_SHADER }, { "tese", GL_TESS_EVALUATION_SHADER },
        { "geom", GL_GEOMETRY_SHADER }, { "frag", GL_FRAGMENT_SHADER },     { "comp", GL_COMPUTE_SHADER },
    };
    auto it = map.find(suffix);
    if (it != map.end()) {
        return it->second;
    } else {
        return GL_UNKNOWN_SHADER;
    }
}

static std::map<std::string, std::unique_ptr<Shader>> ShaderCache;

Shader& GetShader(const std::string& dir, const std::string& source, GLenum type, bool force_compile) {
    static Shader EmptyShader;
    // look up cache
    const auto file = dir + source;
    const auto it = ShaderCache.find(file);
    if (it != ShaderCache.end()) {
        if (force_compile) {
            it->second->Delete();
        } else {
            return *it->second;
        }
    }
    // identify shader type
    if (type == GL_UNKNOWN_SHADER) {
        const size_t beyond_dot_pos = file.rfind('.') + 1;
        type = SuffixType(file.substr(beyond_dot_pos));
        if (type == GL_UNKNOWN_SHADER) {
            DEBUG("Can not identify the shader type of %s", file.substr(beyond_dot_pos).c_str());
            return EmptyShader;
        }
    }
    // fetch source
    std::ifstream file_stream;
    file_stream.open(file.c_str());
    if (!file_stream) {
        DEBUG("Failed to open %s", file.c_str());
        return EmptyShader;
    }
    std::string source_string((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
    // add to cache and compile
    auto&& shader = ShaderCache[file];
    if (!shader) {
        shader.reset(new Shader());
    }
    shader->Create(type);
    shader->Source(source_string.c_str());
    shader->Compile();
    return *shader;
}

} // namespace ShaderResource

} // namespace OpenGL
