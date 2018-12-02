#ifndef OPENGL_LAB_CONSTANTS_HPP
#define OPENGL_LAB_CONSTANTS_HPP

#include "OpenGL/Common.hpp"

#include <string>


namespace OpenGL {

static constexpr GLenum GL_UNKNOWN_SHADER = -1;

GLsizei
sizeOfDataType(unsigned int type);

const char*
nameOfDataType(unsigned int type);

const char*
nameOfShaderType(GLenum type);

GLenum
shaderTypeOfSuffix(std::string suffix);

const char*
nameOfProgramInterface(GLenum interface);

} // namespace OpenGL

#endif //OPENGL_LAB_CONSTANTS_HPP
