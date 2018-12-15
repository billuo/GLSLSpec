/**
 * @File ProgramPipeline.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Object/ProgramPipeline.hpp>


namespace OpenGL {

ProgramPipeline&
ProgramPipeline::use_stage(const Program& program, GLbitfield bits)
{
    glUseProgramStages(name(), bits, program.name());
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

} // namespace OpenGL
