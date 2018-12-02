#ifndef OPENGL_LAB_INTROSPECTOR_HPP
#define OPENGL_LAB_INTROSPECTOR_HPP

#include "Introspection.hpp"


namespace OpenGL {

class Introspector {
    template <typename T> using UP = std::unique_ptr<T>;
  public:
    Introspector(const Program& program);
    ~Introspector() = default;

    const GLuint name; ///< only a weak reference to program obejct
    const std::string label; ///< debug label; could be empty string
    const UP<UniformInterface> IUniform;
    const UP<UniformBlockInterface> IUniformBlock;
    const UP<ProgramInputInterface> IInput;
    const UP<ProgramOutputInterface> IOutput;

    friend std::ostream& operator<<(std::ostream& os, const Introspector& introspector);
};

}
#endif //OPENGL_LAB_INTROSPECTOR_HPP
