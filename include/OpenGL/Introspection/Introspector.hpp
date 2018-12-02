#ifndef OPENGL_LAB_INTROSPECTOR_HPP
#define OPENGL_LAB_INTROSPECTOR_HPP

#include "Introspection.hpp"


namespace OpenGL {

class Introspector {
    template <typename T> using UP = std::unique_ptr<T>;
  public:
    Introspector(const Program& program);
    ~Introspector() = default;

    const UP<UniformInterface> IUniform;
    const UP<UniformBlockInterface> IUniformBlock;
    const UP<ProgramInputInterface> IInput;
    const UP<ProgramOutputInterface> IOutput;

  private:
    GLuint m_name; // only a weak reference
};

}
#endif //OPENGL_LAB_INTROSPECTOR_HPP
