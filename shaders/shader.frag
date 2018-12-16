#version 430 core

#ifdef BACKGROUND

#define PI_TWO 1.5707963
#define PI 3.1415926
#define TWO_PI 6.2831852

uniform ivec2 u_fbsize;
uniform ivec2 u_mpos;
uniform float u_time;

vec3 hsb2rgb(vec3 hsb) {
    vec3 a = mod(fma(hsb.xxx, vec3(6.0f), vec3(0.0f, 4.0f, 2.0f)), 6.0f);
    vec3 rgb = clamp(abs(a-3.0f)-1.0f, 0.0f, 1.0f);
    rgb = rgb*rgb*fma(vec3(-2.0f), rgb, vec3(3.0));
    return hsb.z*mix(vec3(1.0f), rgb, hsb.y);
}

out vec4 o_color;

void main(void) {
    vec2 st = gl_FragCoord.xy / u_fbsize;
    vec2 ij = vec2(u_mpos) / u_fbsize;
    //
    vec2 from_center = st - vec2(0.5f);
    vec2 mouse_from_center = ij - vec2(0.5f);
    float angle = atan(from_center.y, from_center.x);
    float mouse_angle = atan(mouse_from_center.y, mouse_from_center.x);
    float r = length(from_center) * 2.0f;
    float mouse_r = length(mouse_from_center) * 2.0f;
    vec3 color = mix(hsb2rgb(vec3(angle/TWO_PI, r, 1.0f)), hsb2rgb(vec3(mouse_angle/TWO_PI, mouse_r, 1.0f)), step(1.0f, r));
    //
    o_color = vec4(color, 1.0f);
}

#else

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 color;

out vec4 o_color;

void main(void) {
    o_color = vec4(color, 1.0f);
}

#endif
