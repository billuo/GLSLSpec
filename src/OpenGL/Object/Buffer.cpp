#include "OpenGL/Object/Buffer.hpp"


namespace OpenGL {

decltype(Object::makeNamePool(glGenBuffers, glDeleteBuffers))
        Buffer::Pool = Object::makeNamePool(glGenBuffers, glDeleteBuffers);
} // namespace OpenGL
