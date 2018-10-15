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

static std::map<std::string, GLenum> SuffixTypeInitMap() {
    static std::map<std::string, GLenum> map;
    map["vert"] = GL_VERTEX_SHADER;
    map["tesc"] = GL_TESS_CONTROL_SHADER;
    map["tese"] = GL_TESS_EVALUATION_SHADER;
    map["geom"] = GL_GEOMETRY_SHADER;
    map["frag"] = GL_FRAGMENT_SHADER;
    map["comp"] = GL_COMPUTE_SHADER;
    return map;
}

static GLenum SuffixType(const std::string& suffix) {
    static std::map<std::string, GLenum> map = SuffixTypeInitMap();
    std::map<std::string, GLenum>::iterator it = map.find(suffix);
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
            ShaderCache.erase(it);
        } else {
            return *it->second;
        }
    }
    // identify shader type
    if (type == GL_UNKNOWN_SHADER) {
        const size_t beyond_dot_pos = file.rfind('.') + 1;
        type = SuffixType(file.substr(beyond_dot_pos));
        if (type == GL_UNKNOWN_SHADER) {
            DEBUG("Can not identify the shader type of %s\n", file.substr(beyond_dot_pos).c_str());
            return EmptyShader;
        }
    }
    // fetch source
    std::ifstream file_stream;
    file_stream.open(file.c_str());
    if (!file_stream) {
        DEBUG("Failed to open %s\n", file.c_str());
        return EmptyShader;
    }
    std::string source_string((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
    // add to cache and compile
    Shader& shader = *new Shader();
    shader.Create(type);
    shader.Source(source_string.c_str());
    shader.Compile();
    ShaderCache[file].reset(&shader);
    return shader;
}

} // namespace ShaderResource

} // namespace OpenGL
