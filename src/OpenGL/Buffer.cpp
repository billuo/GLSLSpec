#include "OpenGL/Buffer.hpp"

namespace OpenGL {

auto Buffer::Pool = MakeNamePool(glCreateBuffers, glDeleteBuffers);

} // namespace OpenGL
