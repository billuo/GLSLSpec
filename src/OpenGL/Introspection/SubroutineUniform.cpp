#include "OpenGL/Introspection/SubroutineUniform.hpp"


namespace OpenGL {

SubroutineUniform::SubroutineUniform(GLuint program, GLenum stage, GLint index, GLchar* name,
                                     const GLint* values) : Resource(index, name)
{
    asize = values[0];
    location = values[1];
    GLint n_subroutines;
    glGetProgramStageiv(program, stage, GL_ACTIVE_SUBROUTINES, &n_subroutines);
    for (GLint i = 0; i < n_subroutines; ++i) {
        GLint name_length;
        glGetActiveSubroutineName(program, stage, i, 0, &name_length, nullptr);
        auto&& name = std::make_unique<GLchar[]>(name_length);
        glGetActiveSubroutineName(program, stage, i, name_length, nullptr, name.get());
        subroutines.emplace_back(i, name.get());
    }
}

std::ostream&
operator<<(std::ostream& os, const SubroutineUniform& sub_uniform)
{
    os << static_cast<const Resource&>(sub_uniform) << '\n';
    os << "array_size=" << sub_uniform.asize << ", location" << sub_uniform.location;
    for (auto&& sr : sub_uniform.subroutines) {
        os << sr << '\n';
    }
    return os;
}

} // namespace OpenGL
