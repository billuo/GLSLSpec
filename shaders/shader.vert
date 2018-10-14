#version 430 core

/* clang-format off */
out VS_GS {
    vec3 pos;
    vec3 normal;
    vec3 color;
} vs_out;
/* clang-format on */

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

struct light_t {
    vec4 position; // light position [eye]
    vec3 la;       // ambient light intensity
    vec3 ld;       // diffuse light intensity
    vec3 ls;       // specular light intensity
};
uniform light_t Light;

// struct material_t{
//     vec3 ka;
//     vec3 kd;
//     vec3 ks;
//     float shininess;
// };
// uniform material_t Material;

layout(binding = 0) uniform Transformations {
    mat4 NDC_World;
    mat4 World_Model;
};

void main(void) {
    vec4 world = World_Model * vec4(pos, 1.0f);
    gl_Position = NDC_World * world;
    vs_out.pos = world.xyz;
    vs_out.normal = (World_Model * vec4(normal, 0.0f)).xyz;
    vs_out.color = color;
}
