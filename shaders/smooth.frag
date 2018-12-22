#version 430 core

uniform sampler2D u_scene;
uniform sampler2D u_depth;
uniform ivec2 u_fbsize;

in vec2 v_position;
in vec2 v_texcoord;

out vec4 o_color;

vec4 conv(vec2 texcoord, const vec2 delta){
    const vec4 v = vec4(delta, 0.0, -delta.y);
    return (1*texture(u_scene, texcoord-v.xw) + 2*texture(u_scene, texcoord+v.zy) + 1*texture(u_scene, texcoord+v.xy) +
            2*texture(u_scene, texcoord-v.xz) + 4*texture(u_scene, texcoord     ) + 2*texture(u_scene, texcoord+v.xz) +
            1*texture(u_scene, texcoord-v.xy) + 2*texture(u_scene, texcoord-v.zy) + 1*texture(u_scene, texcoord+v.xw))/16;
}

void main() {
    const vec2 step = vec2(1.0) / u_fbsize;
    o_color = vec4(conv(v_texcoord, step).rgb, 1.0);
}
