/**
 * @File Shader.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Introspection/Introspector.hpp>

#include <fstream>


namespace OpenGL {

Shader&
Shader::source(const GLchar** sources, size_t count)
{
    Owned<GLint[]> lens = std::make_unique<GLint[]>(count);
    for (size_t i = 0; i < count; ++i) {
        lens[i] = static_cast<GLint>(strlen(sources[i]));
    }
    glShaderSource(name(), static_cast<GLsizei>(count), sources, lens.get());
    return *this;
}

Shader&
Shader::source(const std::string& source)
{
    auto str = source.c_str();
    auto len = static_cast<GLint>(source.size());
    glShaderSource(name(), 1, &str, &len);
    return *this;
}

Shader&
Shader::compile()
{
    glCompileShader(name());
    if (get(GL_COMPILE_STATUS) == GL_FALSE) {
        Log::e("{} compilation failed:{}",
               nameOfShaderType(get(GL_SHADER_TYPE)),
               get_info_log().get());
    }
    return *this;
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
    ret = std::make_unique<char[]>(length + 1); // XXX one more byte, just in case
    glGetShaderInfoLog(name(), length, nullptr, ret.get());
    return ret;
}

} // namespace OpenGL
