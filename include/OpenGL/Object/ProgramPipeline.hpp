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

    GLuint m_stages[MaxShaderStage] = {};

  public:
    static void Bind(const ProgramPipeline& obj)
    { glBindProgramPipeline(obj.name()); }

    explicit ProgramPipeline(const GLchar* label = nullptr) : Object(pool().get())
    {
    }

    ProgramPipeline(ProgramPipeline&&) = default;
    ProgramPipeline& operator=(ProgramPipeline&&) = default;

    ~ProgramPipeline()
    { pool().put(std::move(m_name)); }

    using Object::label;

    void label(const GLchar* label)
    { Object::label(label, GL_PROGRAM_PIPELINE); }

    GLuint stage(ShaderStage order);

    void use_stage(const Program& program, GLbitfield stages);

    bool valid() const;

    GLint get(GLenum param) const;

    std::unique_ptr<GLchar[]> get_info_log() const;

};

} // namespace OpenGL

