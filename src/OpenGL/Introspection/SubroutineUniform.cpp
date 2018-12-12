#include <OpenGL/Introspection/SubroutineUniform.hpp>


namespace OpenGL {

SubroutineUniform::SubroutineUniform(GLuint program, GLenum stage, GLint index, GLchar* name, const GLint* values)
        : Resource(index, name)
{
    asize = values[0];
    location = values[1];
    GLint n_subroutines;
    GLint name_length;
    glGetProgramStageiv(program, stage, GL_ACTIVE_SUBROUTINES, &n_subroutines);
    glGetProgramStageiv(program, stage, GL_ACTIVE_SUBROUTINE_MAX_LENGTH, &name_length);
    name_length++;
    auto&& buffer = std::make_unique<GLchar[]>(name_length);
    for (GLint i = 0; i < n_subroutines; ++i) {
        GLsizei len;
        glGetActiveSubroutineName(program, stage, i, name_length, &len, buffer.get());
        if (len != 0) {
            subroutines.emplace_back(i, buffer.get());
        }
    }
}

std::ostream&
operator<<(std::ostream& os, const SubroutineUniform& sub_uniform)
{
    os << static_cast<const Resource&>(sub_uniform) << '\n';
    os << "array size=" << sub_uniform.asize << ", location=" << sub_uniform.location << '\n';
    os << "available subroutines {";
    for (auto&& sr : sub_uniform.subroutines) {
        os << "\n\t" << sr;
    }
    os << "\n}";
    return os;
}

} // namespace OpenGL
