#pragma once

// TODO this is platform dependent
#include <glad/glad.h>
// <GLFW/glfw3.h> must be included after "glad/glad.h"
#include <GLFW/glfw3.h>


struct Window;

/// OpenGL C++ wrapper
namespace OpenGL {

void
Initialize();

void
Exit();

} // namespace OpenGL

