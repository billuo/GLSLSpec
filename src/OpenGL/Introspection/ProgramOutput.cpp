#include <OpenGL/Introspection/ProgramOutput.hpp>


namespace OpenGL {

std::ostream&
operator<<(std::ostream& os, const ProgramOutput& output)
{
    os << static_cast<const Resource&>(output) << '\n';
    if (output.per_patch) {
        os << "[per_patch] ";
    }
    os << "type=" << nameOfDataType(output.type) << ", array_size=" << output.asize << '\n';
    os << "location=" << output.location << ", location index=" << output.location_index
       << ", component location=" << output.component << '\n';
    os << Resource::referenced_stages(output.referenced);
    return os;
}

ProgramOutput::ProgramOutput(GLuint program, GLint index, GLchar* name, const GLint* values)
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
