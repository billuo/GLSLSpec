/**
 * @File ProgramPipeline.hpp
 * @brief Encapsulates OpenGL program pipeline object
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Program.hpp"


namespace OpenGL {

/// OpenGL program pipeline object
class ProgramPipeline : Object {
    static auto& pool()
    {
        static auto singleton = makeNamePool(glGenProgramPipelines, glDeleteProgramPipelines);
        return singleton;
    }

  public:
    static void Bind(const ProgramPipeline& obj)
    { glBindProgramPipeline(obj.name()); }

    explicit ProgramPipeline(const GLchar* label = nullptr) : Object(pool().get())
    {
        Bind(*this);
        if (label) {
            Object::label(label, GL_PROGRAM_PIPELINE);
        }
    }

    ProgramPipeline(ProgramPipeline&&) = default;
    ProgramPipeline& operator=(ProgramPipeline&&) = default;

    ~ProgramPipeline()
    { pool().put(std::move(m_name)); }

    void use_stage(const Program& program, GLbitfield stages)
    { glUseProgramStages(name(), stages, program.name()); }

    bool validate() const
    { glValidateProgramPipeline(name()); }

    GLint get(GLenum param) const
    {
        GLint ret = -1;
        glGetProgramPipelineiv(name(), param, &ret);
        return ret;
    }

    std::unique_ptr<GLchar[]> get_info_log() const
    {
        auto size = get(GL_INFO_LOG_LENGTH);
        std::unique_ptr<GLchar[]> ret = std::make_unique<GLchar[]>(size + 1);
        glGetProgramPipelineInfoLog(name(), size, nullptr, ret.get());
        return ret;
    }

};

} // namespace OpenGL

