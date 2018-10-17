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
    vec4 pos; // light position [eye]
    vec3 la;  // ambient light intensity
    vec3 ld;  // diffuse light intensity
    vec3 ls;  // specular light intensity
};

uniform light_t Light;

struct material_t {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform material_t Material;

layout(binding = 2) uniform Transformations {
    mat4 View_Model;
    mat3 NormalMatrix;
    mat4 NDC_View;
    mat4 NDC_Model;
};

void main(void) {
    gl_Position = NDC_Model * vec4(pos, 1.0f);
    // diffuse shading
    vs_out.normal = normalize(NormalMatrix * normal);
    vec4 view_pos = View_Model * vec4(pos, 1.0f);
    vec3 s = normalize(vec3(Light.pos - view_pos));
    if (normal != pos) {
        vs_out.color = vec3(0.0f, 0.0f, 1.0f);
    } else {
        vs_out.color = Light.ld * Material.kd * max(dot(s, vs_out.normal), 0.0f);
    }
}
