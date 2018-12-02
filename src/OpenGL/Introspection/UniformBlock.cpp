#include "OpenGL/Introspection/UniformBlock.hpp"


namespace OpenGL {

UniformBlock::UniformBlock(GLuint program, GLint index, GLchar* name, const GLint* values)
        : Resource(index, name)
{
    //
    // per block property
    binding = values[0];
    size = values[1];
    for (size_t i = 0; i < MaxShaderStage; ++i) {
        referenced[i] = values[3 + i];
    }
    //
    // prepare to iterate uniforms in the block
    const GLint n_uniforms = values[2];
    const GLint n_props = Uniform::n_properties;
    const GLenum* props = Uniform::properties;
    // array of uniform indices
    GLenum active_variables = GL_ACTIVE_VARIABLES;
    auto&& indices = std::make_unique<GLint[]>(n_uniforms);
    glGetProgramResourceiv(program,
                           interface,
                           index,
                           1,
                           &active_variables,
                           n_uniforms,
                           nullptr,
                           indices.get());
    // uniform name
    GLint max_name_length;
    glGetProgramInterfaceiv(program, Uniform::interface, GL_MAX_NAME_LENGTH, &max_name_length);
    auto&& name_buffer = std::make_unique<GLchar[]>(max_name_length);
    // uniform properties
    auto&& value_buffer = std::make_unique<GLint[]>(n_props);
    // iterate
    uniforms.reserve(n_uniforms);
    for (GLint i = 0; i < n_uniforms; ++i) {
        /// OPT simply fetch from interface GL_UNIFORM with known indices[] to avoid duplicate.
        glGetProgramResourceiv(program,
                               Uniform::interface,
                               indices[i],
                               n_props,
                               props,
                               n_props,
                               nullptr,
                               value_buffer.get());
        glGetProgramResourceName(program,
                                 Uniform::interface,
                                 indices[i],
                                 max_name_length,
                                 nullptr,
                                 name_buffer.get());
        uniforms.emplace_back(program, indices[i], name_buffer.get(), value_buffer.get());
    }
}

const Uniform*
UniformBlock::find(const char* name) const
{
    std::string str(name);
    for (auto& u : uniforms) {
        if (u.name == str) {
            return &u;
        }
    }
    return nullptr;
}

std::ostream&
operator<<(std::ostream& os, const UniformBlock& block)
{
    os << static_cast<const Resource&>(block) << '\n';
    os << "binding=" << block.binding << ", size=" << block.size << "\n";
    os << Resource::referenced_stages(block.referenced) << "\n{";
    for (auto&& u : block.uniforms) {
        os << "\n\t" << static_cast<const Resource&>(u);
        os << "\n\t" << "type=" << nameOfDataType(u.type) << ", offset=" << u.offset << "\n";
    }
    os << '}';
    return os;
}

}
