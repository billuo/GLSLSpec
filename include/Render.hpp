#ifndef RENDER_HPP_1PYP75HW
#define RENDER_HPP_1PYP75HW
#pragma once

#include "glm/fwd.hpp"


namespace Render {

void
init();

void
render();

void
setProjectionMatrix(const glm::mat4& mat);

void
setViewMatrix(const glm::mat4& mat);

enum class Side {
    Front, Back
};

void
toggleAxes();

void
switchRasterizationMode(Side side);

extern float Shininess;
} // namespace Render

#endif /* end of include guard: RENDER_HPP_1PYP75HW */
