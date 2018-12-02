#include "OpenGL/Introspection/Introspector.hpp"


namespace OpenGL {

Introspector::Introspector(const Program& program)
        : m_name(program.name()), IUniform(std::make_unique<UniformInterface>(program)),
          IUniformBlock(std::make_unique<UniformBlockInterface>(program)),
          IInput(std::make_unique<ProgramInputInterface>(program)),
          IOutput(std::make_unique<ProgramOutputInterface>(program))
{}

std::ostream&
operator<<(std::ostream& os, const Introspector& introspector)
{
    static const char* const separator = "##########\n";
    os << "Introspecting Program " << introspector.m_name << std::endl;
    os << "Uniform " << *introspector.IUniform << std::endl;
    os << separator;
    os << "Uniform Block " << *introspector.IUniformBlock << std::endl;
    os << separator;
    os << "Program Input " << *introspector.IInput << std::endl;
    os << separator;
    os << "Program Output " << *introspector.IOutput << std::endl;
    os << "End of Program Introspection";
    return os;
}

}
