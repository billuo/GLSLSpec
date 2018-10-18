#ifndef RENDER_HPP_1PYP75HW
#define RENDER_HPP_1PYP75HW
#pragma once

#include <glm/fwd.hpp>

namespace Render {

void Init();
void Render();

void SetProjectionMatrix(const glm::mat4& mat);
void SetViewMatrix(const glm::mat4& mat);

enum class Side { Front, Back };
void ToggleAxes();
void SwitchRasterizationMode(Side side);

extern float Shininess;

} // namespace Render

#endif /* end of include guard: RENDER_HPP_1PYP75HW */
