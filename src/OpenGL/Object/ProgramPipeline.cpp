/**
 * @File ProgramPipeline.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Object/ProgramPipeline.hpp>
#include <OpenGL/Constants.hpp>


namespace OpenGL {

ProgramPipeline&
ProgramPipeline::use_stage(const Program& program, GLbitfield stages)
{
    auto n = underlying_cast(stageOfShaderBit(stages));
    m_stages[n] = program.name();
    glUseProgramStages(name(), stages, program.name());
    return *this;
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
ProgramPipeline::stage(ShaderStage stage) const
{ return m_stages[underlying_cast(stage)]; }

} // namespace OpenGL
