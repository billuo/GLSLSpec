/**
 * @File ProgramPipeline.hpp
 * @brief Encapsulates OpenGL program pipeline object
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include "Program.hpp"
#include "../Constants.hpp"


namespace OpenGL {

/// OpenGL program pipeline object
class ProgramPipeline : Object {
    static auto& pool()
    {
        static auto singleton = makeNamePool(glGenProgramPipelines, glDeleteProgramPipelines);
        return singleton;
    }

    GLuint m_stages[MaxShaderStage] = {};

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

    GLuint stage(GLenum order);

    void use_stage(const Program& program, GLbitfield stages);

    bool valid() const;

    GLint get(GLenum param) const;

    std::unique_ptr<GLchar[]> get_info_log() const;

};

} // namespace OpenGL

