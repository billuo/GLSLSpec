#include <OpenGL/Object/ProgramPipeline.hpp>
#include <OpenGL/Constants.hpp>


namespace OpenGL {

void
ProgramPipeline::use_stage(const Program& program, GLbitfield stages)
{
    m_stages[orderOfShaderBit(stages)] = program.name();
    glUseProgramStages(name(), stages, program.name());
}

std::unique_ptr<GLchar[]>
ProgramPipeline::get_info_log() const
{
    auto size = get(GL_INFO_LOG_LENGTH);
    std::unique_ptr<GLchar[]> ret = std::make_unique<GLchar[]>(size + 1);
    glGetProgramPipelineInfoLog(name(), size, nullptr, ret.get());
    return ret;
}

GLint
ProgramPipeline::get(GLenum param) const
{
    GLint ret = -1;
    glGetProgramPipelineiv(name(), param, &ret);
    return ret;
}

bool
ProgramPipeline::valid() const
{
    glValidateProgramPipeline(name());
    return get(GL_VALIDATE_STATUS) == GL_TRUE;
}

GLuint
ProgramPipeline::stage(ShaderStage order)
{ return m_stages[order]; }

} // namespace OpenGL
