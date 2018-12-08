#ifndef OPENGL_LAB_CONSTANTS_HPP
#define OPENGL_LAB_CONSTANTS_HPP

#include "Expected.hpp"
#include "OpenGL/Common.hpp"

#include <string>


namespace OpenGL {

GLsizei
sizeOfDataType(GLenum type);

GLsizei
numelOfDataType(GLenum type);

const char*
nameOfDataType(GLenum type);

expected<GLenum, std::string>
shaderTypeOfSuffix(std::string suffix);

const char*
nameOfShaderType(GLenum type);

const char*
nameOfProgramInterface(GLenum interface);

} // namespace OpenGL

#endif //OPENGL_LAB_CONSTANTS_HPP
