/**
 * @File Common.hpp
 * @basic Provide entry point to OpenGL API.
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

// TODO make it platform independent
#include <glad/glad.h>
// XXX <GLFW/glfw3.h> must be included after "glad/glad.h"
#include <GLFW/glfw3.h>

/// OpenGL C++ wrapper
namespace OpenGL {

void
Initialize();

void
Exit();

} // namespace OpenGL

