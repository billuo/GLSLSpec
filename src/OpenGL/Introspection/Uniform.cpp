#include "OpenGL/Introspection/Uniform.hpp"


namespace OpenGL {
Uniform::Uniform(GLuint program, GLint index, GLchar* name, const GLint* values) : Resource(index,
                                                                                            name)
{
    UNUSED(program);
    for (size_t i = 0; i < n_fields; ++i) {
        this->*fields[i] = values[i];
    }
    for (size_t i = 0; i < MaxShaderStage; ++i) {
        referenced[i] = values[n_fields + i];
    }
}

std::ostream&
operator<<(std::ostream& os, const Uniform& uniform)
{
    os << static_cast<const Resource&>(uniform) << '\n';
    if (uniform.location == -1) {
        os << "type=" << nameOfDataType(uniform.type) << ", block_index=" << uniform.block_index
           << '\n';
    } else {
        os << "type=" << nameOfDataType(uniform.type) << ", location=" << uniform.location << '\n';
    }
    os << Resource::referenced_stages(uniform.referenced);
    return os;
}

} // namespace OpenGL
