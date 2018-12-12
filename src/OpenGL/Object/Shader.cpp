#include <OpenGL/Object/Shader.hpp>
#include <OpenGL/Constants.hpp>
#include <Utility/Log.hpp>

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
    if (get(GL_COMPILE_STATUS) == GL_FALSE) {
        Log::e("{} compilation failed:{}",
               nameOfShaderType(get(GL_SHADER_TYPE)),
               get_info_log().get());
    }
}

GLint
Shader::get(GLenum param) const
{
    GLint result = -1;
    glGetShaderiv(name(), param, &result);
    return result;
}

std::unique_ptr<char[]>
Shader::get_info_log() const
{
    std::unique_ptr<char[]> ret;
    GLint length = get(GL_INFO_LOG_LENGTH);
    // XXX one more byte, just in case
    ret = std::make_unique<char[]>(length + 1);
    glGetShaderInfoLog(name(), length, nullptr, ret.get());
    return ret;
}

} // namespace OpenGL
