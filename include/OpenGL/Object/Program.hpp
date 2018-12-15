/**
 * @file Program.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Shader.hpp>
#include <Utility/Misc.hpp>
#include <set>


namespace OpenGL {

class Introspector;

/// OpenGL shader program object
class Program : public Object {
    static auto& pool()
    {
        static struct {
            Name get()
            {
                GLuint name = glCreateProgram();
                return Name(name);
            }

            void put(Name&& name)
            { glDeleteProgram(name.get()); }
        } singleton;
        return singleton;
    }

    static Name Standalone(GLenum type, std::initializer_list<std::string> sources)
    {
        if (sources.size() == 0) {
            return Name();
        }
        Owned<const GLchar* []> strings = std::make_unique<const GLchar* []>(sources.size());
        auto it = sources.begin();
        for (size_t i = 0; i < sources.size(); ++i) {
            strings[i] = it++->c_str();
        }
        auto ret = glCreateShaderProgramv(type, 1, strings.get());
        if (ret == 0) {
            Log::e("Failed to create shader");
        }
        return Name(ret);
    }

  public:
    static void Use(const Program& prog)
    { glUseProgram(prog.name()); }

    explicit Program() : Object(pool().get())
    {}

    explicit Program(GLenum type, std::initializer_list<std::string> sources) : Object(Standalone(type, sources))
    {}

    Program(Program&&) = default;
    Program& operator=(Program&&) = default;

    ~Program();

    using Object::label;

    Program& label(const std::string& label)
    { return this->label(label.c_str()); }

    Program& label(const GLchar* label)
    {
        Object::label(label, GL_PROGRAM);
        return *this;
    }

    Program& attach(const Shader& shader);

    /// Link all attached shaders together, forming a valid program.
    Program& link();

    Program& use()
    {
        Use(*this);
        return *this;
    }

    Weak<Introspector> interfaces() const;

    /// Query about a parameter
    GLint get(GLenum param) const;
    /// Query about a property an interface
    GLint get_stage(GLenum stage, GLenum pname) const;
    /// Retrive information log safely.
    std::unique_ptr<GLchar[]> get_info_log() const;
    /// Set a parameter.
    Program& set(GLenum param, GLint value);
};

} // namespace OpenGL

