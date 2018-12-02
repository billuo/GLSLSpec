#include "OpenGL/Introspection/Introspector.hpp"


namespace OpenGL {

Introspector::Introspector(const Program& program)
        : name(program.name()),
          label(program.label()),
          IUniform(std::make_unique<UniformInterface>(name)),
          IUniformBlock(std::make_unique<UniformBlockInterface>(name)),
          IInput(std::make_unique<ProgramInputInterface>(name)),
          IOutput(std::make_unique<ProgramOutputInterface>(name))
{}

std::ostream&
operator<<(std::ostream& os, const Introspector& introspector)
{
    static const char* const separator = "##########\n";
    os << "Introspecting Program " << introspector.label << '[' << introspector.name << "]\n";
    os << separator;
    os << "Uniform " << *introspector.IUniform << std::endl;
    os << separator;
    os << "Uniform Block " << *introspector.IUniformBlock << std::endl;
    os << separator;
    os << "Program Input " << *introspector.IInput << std::endl;
    os << separator;
    os << "Program Output " << *introspector.IOutput << std::endl;
    os << separator;
    os << "End of Program Introspection";
    return os;
}

}
