#version 430 core

#if defined(BACKGROUND)

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

void main() {
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
#elif defined(POSTPROCESS)

#define PI 3.1415926

uniform sampler2D u_scene;
uniform sampler2D u_depth;
uniform float u_time;

in vec2 v_position;
in vec2 p_texcoord;

out vec4 o_color;

vec3 color_floor(vec3 p)
{
    if (fract(p.x*0.2)>0.2) {
        if (fract(p.z*0.2) > 0.2) {
            return vec3(0.0f, 0.1f, 0.2f);
        }
        return vec3(1.0f, 1.0f, 1.0f);
    } else {
        if (fract(p.z*0.2) > 0.2) {
            return vec3(1.0f, 1.0f, 1.0f);
        }
        return vec3(0.3f, 0.0f, 0.0f);
    }
}

vec3 color_primitive(vec3 p)
{
    return vec3(0.6f, 0.6f, 0.8f);
}

vec2 obj_floor(vec3 p)
{
    return vec2(p.y + 10.0, 0);
}

vec2 distance_to_obj(vec3 p)
{
    return obj_floor(p);
}

void main() {
    vec2 tex = p_texcoord;
    tex.x += 0.02f * cos(tex.y * 8 * PI + u_time * PI);
    float sqrt2over2 = 1.414f /2f;
    vec2 shifted = tex + mat2(2*sqrt2over2, 2*sqrt2over2, -sqrt2over2, sqrt2over2) * vec2(0.01f * sin(u_time), 0.01f* cos(u_time));
    vec3 color = mix(texture(u_scene, shifted).rgb, texture(u_scene, tex).rgb, 0.5f);
    color = max(color, 0.3f * vec3(abs(sin(u_time))));
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
