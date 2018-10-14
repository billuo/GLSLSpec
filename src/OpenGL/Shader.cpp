#include <cstring>
#include <vector>

#include "OpenGL/Shader.hpp"

namespace OpenGL {

const char* Shader::NameOfType(GLenum type) {
    static const struct {
        GLenum type;
        const char* const name;
    } types[] = {
#define TYPE_NAME_PAIR(type) { type, #type }
        TYPE_NAME_PAIR(GL_VERTEX_SHADER),          TYPE_NAME_PAIR(GL_TESS_CONTROL_SHADER),
        TYPE_NAME_PAIR(GL_TESS_EVALUATION_SHADER), TYPE_NAME_PAIR(GL_GEOMETRY_SHADER),
        TYPE_NAME_PAIR(GL_FRAGMENT_SHADER),        TYPE_NAME_PAIR(GL_COMPUTE_SHADER),
#undef TYPE_NAME_PAIR
    };
    for (size_t i = 0; i < countof(types); ++i) {
        if (types[i].type == type)
            return types[i].name;
    }
    return "Unknown";
}

void Shader::Source(const GLchar** sources, size_t count) {
    if (Initialized()) {
        std::vector<GLint> lens(count);
        for (size_t i = 0; i < count; ++i) {
            lens[i] = strlen(sources[i]);
        }
        glShaderSource(Name(), count, sources, lens.data());
    }
}

void Shader::Compile() {
    if (Initialized()) {
        glCompileShader(Name());
        if (aux_GetParameter(GL_COMPILE_STATUS) == GL_FALSE) {
            DEBUG("%s compilation failed:%s", NameOfType(aux_GetParameter(GL_SHADER_TYPE)), GetInfoLog().c_str());
        }
    }
}

GLint Shader::aux_GetParameter(GLenum pname) const {
    GLint result = -1;
    if (Initialized()) {
        glGetShaderiv(Name(), pname, &result);
    }
    return result;
}

std::string Shader::GetInfoLog() const {
    std::string ret;
    if (Initialized()) {
        GLint length = aux_GetParameter(GL_INFO_LOG_LENGTH);
        assert(length >= 0);
        ret.resize(length);
        glGetShaderInfoLog(Name(), length, NULL, &ret[0]);
    }
    return ret;
}

} // namespace OpenGL
