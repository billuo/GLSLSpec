/**
 * @File ProgramInput.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Introspection/ProgramInput.hpp>


namespace OpenGL {

std::ostream&
operator<<(std::ostream& os, const ProgramInput& input)
{
    os << static_cast<const Resource&>(input) << '\n';
    if (input.per_patch) {
        os << "[per_patch] ";
    }
    os << "type=" << nameOfDataType(input.type) << ", array_size=" << input.asize << '\n';
    os << "location=" << input.location << ", component location=" << input.component << '\n';
    os << Resource::referenced_stages(input.referenced);
    return os;
}

ProgramInput::ProgramInput(GLuint program, GLint index, GLchar* name, const GLint* values)
        : Resource(index, name)
{
    for (size_t i = 0; i < n_fields; ++i) {
        this->*fields[i] = values[i];
    }
    for (size_t i = 0; i < MaxShaderStage; ++i) {
        referenced[i] = values[n_fields + i];
    }
}

} // namespace OpenGL
