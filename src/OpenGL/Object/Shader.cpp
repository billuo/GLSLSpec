#include "OpenGL/Object/Shader.hpp"
#include "OpenGL/Constants.hpp"
#include "Log.hpp"

#include <cstring>
#include <fstream>


namespace OpenGL {

void
Shader::source(const GLchar** sources, size_t count)
{
    std::vector<GLint> lens(count);
    for (size_t i = 0; i < count; ++i) {
        lens[i] = static_cast<int>(strlen(sources[i]));
    }
    glShaderSource(name(), static_cast<GLsizei>(count), sources, lens.data());
}

void
Shader::compile()
{
    glCompileShader(name());
    if (aux_GetParameter(GL_COMPILE_STATUS) == GL_FALSE) {
        Log::e("%s compilation failed:%s",
               nameOfShaderType(aux_GetParameter(GL_SHADER_TYPE)),
               aux_GetInfoLog().get());
    }
}

GLint
Shader::aux_GetParameter(GLenum pname) const
{
    GLint result = -1;
    glGetShaderiv(name(), pname, &result);
    return result;
}

std::unique_ptr<char[]>
Shader::aux_GetInfoLog() const
{
    std::unique_ptr<char[]> ret;
    GLint length = aux_GetParameter(GL_INFO_LOG_LENGTH);
    // XXX one more byte, just in case
    ret = std::make_unique<char[]>(length + 1);
    glGetShaderInfoLog(name(), length, nullptr, ret.get());
    return ret;
}

} // namespace OpenGL
