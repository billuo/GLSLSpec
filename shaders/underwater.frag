#version 430 core

#if defined(POSTPROCESS)

#define PI 3.1415926

uniform sampler2D u_scene;
uniform float u_time;
uniform ivec4 u_viewport;

in vec2 v_position;
in vec2 v_texcoord;

out vec4 o_color;

void main() {
    vec2 tex = v_texcoord;
    vec2 pixel = vec2(1.0) / u_viewport.zw;
    tex.x += 3 * pixel.x * cos(tex.y * 4 * PI + u_time * PI);
    float sqrt2over2 = 1.414f /2f;
    vec2 shifted = tex +
        mat2(2*sqrt2over2, 2*sqrt2over2, -sqrt2over2, sqrt2over2) *
        vec2(sin(u_time), cos(u_time)) * pixel * 4.0;
    vec3 color = mix(texture(u_scene, shifted).rgb, texture(u_scene, tex).rgb, 0.3f);
    o_color = vec4(color, 1.0f);
}

#endif

