#version 430 core

#if defined(POSTPROCESS)

uniform sampler2D u_scene;
uniform ivec2 u_fbsize;
uniform float u_time;

in vec2 v_position;
in vec2 v_texcoord;

out vec4 o_color;

void draw_hatch(inout vec3 hatch_color, vec3 color, float threshold, vec2 xy, float fx, float density) {
    if (dot(color, vec3(0.2126, 0.7152, 0.0722)) > threshold) {
        return;
    }
    fx = mod(fx, density);
    const float thickness = 1.6;
    float draw = smoothstep(-thickness, 0.0, fx) - smoothstep(0.0, thickness, fx);
    hatch_color = mix(hatch_color, vec3(0.0, 0.0, 0.0), draw);
}

vec3 cross_hatch(vec3 color) {
    const float density = 12.0;
    vec3 ret = vec3(1.0);
    vec2 pos = vec2(floor(gl_FragCoord.x), floor(gl_FragCoord.y));
    draw_hatch(ret, color, 0.8, pos, pos.x + pos.y, density);
    draw_hatch(ret, color, 0.6, pos, pos.x - pos.y, density);
    draw_hatch(ret, color, 0.4, pos, pos.x + pos.y + density / 2.0, density);
    draw_hatch(ret, color, 0.2, pos, pos.x - pos.y + density / 2.0, density);
    return ret;
}

void main() {
    vec2 uv = v_texcoord;
    vec3 color = texture(u_scene, uv).rgb;
    o_color = vec4(mix(cross_hatch(color), color, step(abs(sin(u_time)), uv.x)), 1.0);
}

#endif
