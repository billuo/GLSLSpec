#version 430 core

in vec3 color;
out vec4 color_out;

void main(void) { color_out = vec4(color, 1.0f); }
