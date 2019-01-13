#version 430 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 color;

out vec4 o_color;

void main(void) {
    o_color = vec4(color, 1.0f);
}

