#ifndef OPENGL_LAB_INTROSPECTOR_HPP
#define OPENGL_LAB_INTROSPECTOR_HPP

#include "Interface.hpp"


namespace OpenGL {

class Introspector {
    template <typename T> using UP = std::unique_ptr<T>;
  public:
    explicit Introspector(const Program& program);
    ~Introspector() = default;

    /// A weak reference to program object
    const GLuint name;
    /// debug label; could be empty string
    const std::string label;

    const ProgramInterface<Uniform>& uniform() const
    {
        if (!IUniform) {
            IUniform = std::make_unique<UniformInterface>(name);
        }
        return *IUniform;
    }

    const ProgramInterface<UniformBlock>& uniform_block() const
    {
        if (!IUniformBlock) {
            IUniformBlock = std::make_unique<UniformBlockInterface>(name);
        }
        return *IUniformBlock;
    }

    const ProgramInterface<ProgramInput>& input() const
    {
        if (!IInput) {
            IInput = std::make_unique<ProgramInputInterface>(name);
        }
        return *IInput;
    }

    const ProgramInterface<ProgramOutput>& output() const
    {
        if (!IOutput) {
            IOutput = std::make_unique<ProgramOutputInterface>(name);
        }
        return *IOutput;
    }

    const ProgramInterface<VertexSubroutineUniform>& vertex_subroutine_uniform() const
    {
        if (!IVertexSubroutineUniform) {
            IVertexSubroutineUniform = std::make_unique<VertexSubroutineUniformInterface>(name);
        }
        return *IVertexSubroutineUniform;
    }

    const ProgramInterface<TessControlSubroutineUniform>& tess_control_subroutine_uniform() const
    {
        if (!ITessControlSubroutineUniform) {
            ITessControlSubroutineUniform = std::make_unique<TessControlSubroutineUniformInterface>(name);
        }
        return *ITessControlSubroutineUniform;
    }

    const ProgramInterface<TessEvaluationSubroutineUniform>& tess_evaluation_subroutine_uniform() const
    {
        if (!ITessEvaluationSubroutineUniform) {
            ITessEvaluationSubroutineUniform = std::make_unique<TessEvaluationSubroutineUniformInterface>(name);
        }
        return *ITessEvaluationSubroutineUniform;
    }

    const ProgramInterface<GeometrySubroutineUniform>& geometry_subroutine_uniform() const
    {
        if (!IGeometrySubroutineUniform) {
            IGeometrySubroutineUniform = std::make_unique<GeometrySubroutineUniformInterface>(name);
        }
        return *IGeometrySubroutineUniform;
    }

    const ProgramInterface<FragmentSubroutineUniform>& fragment_subroutine_uniform() const
    {
        if (!IFragmentSubroutineUniform) {
            IFragmentSubroutineUniform = std::make_unique<FragmentSubroutineUniformInterface>(name);
        }
        return *IFragmentSubroutineUniform;
    }

    const ProgramInterface<ComputeSubroutineUniform>& compute_subroutine_uniform() const
    {
        if (!IComputeSubroutineUniform) {
            IComputeSubroutineUniform = std::make_unique<ComputeSubroutineUniformInterface>(name);
        }
        return *IComputeSubroutineUniform;
    }

  private:
    mutable UP<UniformInterface> IUniform;
    mutable UP<UniformBlockInterface> IUniformBlock;
    mutable UP<ProgramInputInterface> IInput;
    mutable UP<ProgramOutputInterface> IOutput;
    mutable UP<VertexSubroutineUniformInterface> IVertexSubroutineUniform;
    mutable UP<TessControlSubroutineUniformInterface> ITessControlSubroutineUniform;
    mutable UP<TessEvaluationSubroutineUniformInterface> ITessEvaluationSubroutineUniform;
    mutable UP<GeometrySubroutineUniformInterface> IGeometrySubroutineUniform;
    mutable UP<FragmentSubroutineUniformInterface> IFragmentSubroutineUniform;
    mutable UP<ComputeSubroutineUniformInterface> IComputeSubroutineUniform;

    friend std::ostream& operator<<(std::ostream& os, const Introspector& introspector);
};

}
#endif //OPENGL_LAB_INTROSPECTOR_HPP
