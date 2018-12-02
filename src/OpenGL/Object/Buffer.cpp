#include "OpenGL/Object/Buffer.hpp"


namespace OpenGL {

decltype(Object::makeNamePool(glCreateBuffers, glDeleteBuffers))
        Buffer::Pool = Object::makeNamePool(glCreateBuffers, glDeleteBuffers);
} // namespace OpenGL
