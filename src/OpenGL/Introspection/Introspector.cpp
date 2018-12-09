#include "OpenGL/Introspection/Introspector.hpp"


namespace OpenGL {

Introspector::Introspector(const Program& program)
        : name(program.name()),
          label(program.label()),
          IUniform(std::make_unique<UniformInterface>(name)),
          IUniformBlock(std::make_unique<UniformBlockInterface>(name)),
          IInput(std::make_unique<ProgramInputInterface>(name)),
          IOutput(std::make_unique<ProgramOutputInterface>(name)),
          IVertexSubroutineUniform(std::make_unique<VertexSubroutineUniformInterface>(name)),
          ITessControlSubroutineUniform(std::make_unique<TessControlSubroutineUniformInterface>(name)),
          ITessEvaluationSubroutineUniform(std::make_unique<TessEvaluationSubroutineUniformInterface>(name)),
          IGeometrySubroutineUniform(std::make_unique<GeometrySubroutineUniformInterface>(name)),
          IFragmentSubroutineUniform(std::make_unique<FragmentSubroutineUniformInterface>(name)),
          IComputeSubroutineUniform(std::make_unique<ComputeSubroutineUniformInterface>(name))
{}

std::ostream&
operator<<(std::ostream& os, const Introspector& introspector)
{
    static const char* const separator = "\n##########\n";
    os << "Introspecting Program " << introspector.label << '[' << introspector.name << "]";
    os << separator;
    os << "Uniform " << *introspector.IUniform;
    os << separator;
    os << "Uniform Block " << *introspector.IUniformBlock;
    os << separator;
    os << "Program Input " << *introspector.IInput;
    os << separator;
    os << "Program Output " << *introspector.IOutput;
    os << separator;
    os << "Vertex Shader Subroutine Uniform " << *introspector.IVertexSubroutineUniform;
    os << separator;
    os << "Tessellation Control Shader Subroutine Uniform " << *introspector.ITessControlSubroutineUniform;
    os << separator;
    os << "Tessellation Evaluation Shader Subroutine Uniform " << *introspector.ITessEvaluationSubroutineUniform;
    os << separator;
    os << "Geometry Shader Subroutine Uniform " << *introspector.IGeometrySubroutineUniform;
    os << separator;
    os << "Fragment Shader Subroutine Uniform " << *introspector.IFragmentSubroutineUniform;
    os << separator;
    os << "Compute Shader Subroutine Uniform " << *introspector.IComputeSubroutineUniform;
    os << separator;
    os << "End of Program Introspection";
    return os;
}

}
