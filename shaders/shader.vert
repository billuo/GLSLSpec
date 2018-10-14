#version 430 core

/* clang-format off */
out VS_GS {
    vec3 pos;
    vec3 normal;
    vec3 color;
}
vs_out;
/* clang-format on */

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

layout(binding = 0) uniform Transformations {
    mat4 NDC_World;
    mat4 World_Model;
};

// layout(location = 0) uniform mat4 NDC_World = mat4(1.0f);
// layout(location = 1) uniform mat4 World_Model = mat4(1.0f);

void main(void) {
    vec4 world = World_Model * vec4(pos, 1.0f);
    gl_Position = NDC_World * world;
    vs_out.pos = world.xyz;
    vs_out.normal = (World_Model * vec4(normal, 0.0f)).xyz;
    vs_out.color = color;
}
