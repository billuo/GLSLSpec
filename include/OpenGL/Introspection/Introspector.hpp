#ifndef OPENGL_LAB_INTROSPECTOR_HPP
#define OPENGL_LAB_INTROSPECTOR_HPP

#include "Interface.hpp"


namespace OpenGL {

class Introspector {
    template <typename T> using UP = std::unique_ptr<T>;
  public:
    explicit Introspector(const Program& program);
    ~Introspector() = default;

    /// A weak reference to program obejct
    const GLuint name;
    /// debug label; could be empty string
    const std::string label;
    const UP<UniformInterface> IUniform;
    const UP<UniformBlockInterface> IUniformBlock;
    const UP<ProgramInputInterface> IInput;
    const UP<ProgramOutputInterface> IOutput;
    const UP<VertexSubroutineUniformInterface> IVertexSubroutineUniform;
    const UP<TessControlSubroutineUniformInterface> ITessControlSubroutineUniform;
    const UP<TessEvaluationSubroutineUniformInterface> ITessEvaluationSubroutineUniform;
    const UP<GeometrySubroutineUniformInterface> IGeometrySubroutineUniform;
    const UP<FragmentSubroutineUniformInterface> IFragmentSubroutineUniform;
    const UP<ComputeSubroutineUniformInterface> IComputeSubroutineUniform;

    friend std::ostream& operator<<(std::ostream& os, const Introspector& introspector);
};

}
#endif //OPENGL_LAB_INTROSPECTOR_HPP
