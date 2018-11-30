#include "OpenGL/Buffer.hpp"


namespace OpenGL {

decltype(Object::MakeNamePool(glCreateBuffers, glDeleteBuffers)) Buffer::Pool = Object::MakeNamePool(glCreateBuffers,
                                                                                                     glDeleteBuffers);
} // namespace OpenGL
