#include <OpenGL/Introspection/Introspector.hpp>


namespace OpenGL {

Introspector::Introspector(const Program& program) : name(program.name()), label(program.label())
{}

std::ostream&
operator<<(std::ostream& os, const Introspector& introspector)
{
    static const char* const separator = "\n##########\n";
    os << "Introspecting Program " << introspector.label << '[' << introspector.name << "]";
    os << separator;
    if (introspector.IUniform) {
        os << "Uniform " << *introspector.IUniform;
        os << separator;
    }
    if (introspector.IUniformBlock) {
        os << "Uniform Block " << *introspector.IUniformBlock;
        os << separator;
    }
    if (introspector.IInput) {
        os << "Program Input " << *introspector.IInput;
        os << separator;
    }
    if (introspector.IOutput) {
        os << "Program Output " << *introspector.IOutput;
        os << separator;
    }
    if (introspector.IVertexSubroutineUniform) {
        os << "Vertex Shader Subroutine Uniform " << *introspector.IVertexSubroutineUniform;
        os << separator;
    }
    if (introspector.ITessControlSubroutineUniform) {
        os << "Tessellation Control Shader Subroutine Uniform " << *introspector.ITessControlSubroutineUniform;
        os << separator;
    }
    if (introspector.ITessEvaluationSubroutineUniform) {
        os << "Tessellation Evaluation Shader Subroutine Uniform " << *introspector.ITessEvaluationSubroutineUniform;
        os << separator;
    }
    if (introspector.IGeometrySubroutineUniform) {
        os << "Geometry Shader Subroutine Uniform " << *introspector.IGeometrySubroutineUniform;
        os << separator;
    }
    if (introspector.IFragmentSubroutineUniform) {
        os << "Fragment Shader Subroutine Uniform " << *introspector.IFragmentSubroutineUniform;
        os << separator;
    }
    if (introspector.IComputeSubroutineUniform) {
        os << "Compute Shader Subroutine Uniform " << *introspector.IComputeSubroutineUniform;
        os << separator;
    }
    os << "End of Program Introspection";
    return os;
}

}
