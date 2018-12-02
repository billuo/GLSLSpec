#include "OpenGL/Introspection/Introspector.hpp"


namespace OpenGL {

Introspector::Introspector(const Program& program) :
        m_name(program.name()),
        IUniform(std::make_unique<UniformInterface>(program)),
        IUniformBlock(std::make_unique<UniformBlockInterface>(program)),
        IInput(std::make_unique<ProgramInputInterface>(program)),
        IOutput(std::make_unique<ProgramOutputInterface>(program))
{}

}
