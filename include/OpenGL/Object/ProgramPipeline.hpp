/**
 * @File ProgramPipeline.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Program.hpp>
#include <OpenGL/Constants.hpp>


namespace OpenGL {

/// OpenGL program pipeline object
class ProgramPipeline : Object {
    static auto& pool()
    {
        static auto singleton = make_pool(glGenProgramPipelines, glDeleteProgramPipelines);
        return singleton;
    }

  public:
    static void Bind(const ProgramPipeline& obj)
    { glBindProgramPipeline(obj.name()); }

    explicit ProgramPipeline(const GLchar* label = nullptr) : Object(pool().get())
    {}

    ProgramPipeline(ProgramPipeline&&) = default;
    ProgramPipeline& operator=(ProgramPipeline&&) = default;

    ~ProgramPipeline()
    { pool().put(std::move(m_name)); }

    using Object::label;

    ProgramPipeline& label(const GLchar* label)
    {
        Object::label(label, GL_PROGRAM_PIPELINE);
        return *this;
    }

    ProgramPipeline& use_stage(const Program& program, GLbitfield bits);

    bool valid() const;

    ProgramPipeline& bind()
    {
        Bind(*this);
        return *this;
    }

    const ProgramPipeline& bind() const
    {
        Bind(*this);
        return *this;
    }

    GLint get(GLenum param) const;

    std::unique_ptr<GLchar[]> get_info_log() const;

};

} // namespace OpenGL

