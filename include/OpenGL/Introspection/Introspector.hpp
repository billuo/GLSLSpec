/**
 * @File Introspector.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Introspection/Interface.hpp>


namespace OpenGL {

class Introspector {
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
    mutable Owned<UniformInterface> IUniform;
    mutable Owned<UniformBlockInterface> IUniformBlock;
    mutable Owned<ProgramInputInterface> IInput;
    mutable Owned<ProgramOutputInterface> IOutput;
    mutable Owned<VertexSubroutineUniformInterface> IVertexSubroutineUniform;
    mutable Owned<TessControlSubroutineUniformInterface> ITessControlSubroutineUniform;
    mutable Owned<TessEvaluationSubroutineUniformInterface> ITessEvaluationSubroutineUniform;
    mutable Owned<GeometrySubroutineUniformInterface> IGeometrySubroutineUniform;
    mutable Owned<FragmentSubroutineUniformInterface> IFragmentSubroutineUniform;
    mutable Owned<ComputeSubroutineUniformInterface> IComputeSubroutineUniform;

    friend std::ostream& operator<<(std::ostream& os, const Introspector& introspector);
};

} // namespace OpenGL
