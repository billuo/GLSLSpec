#version 430 core

/* clang-format off */
out VS_GS {
    vec3 pos;
    vec3 color;
} vs_out;
/* clang-format on */

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;

struct light_t {
    vec3 pos; // light position [eye coord.]
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

uniform Transformations {
    mat4 View_Model;
    mat3 NormalMatrix;
    mat4 NDC_View;
    mat4 NDC_Model;
};

void ViewSpace(out vec3 position, out vec3 normal) {
    position = (View_Model * vec4(VertexPosition, 1.0f)).xyz;
    normal = normalize(NormalMatrix * VertexNormal);
}
void NDCSpace(out vec4 position) { position = NDC_Model * vec4(VertexPosition, 1.0f); }

vec3 ADS(vec3 pos, vec3 norm) {
    vec3 l = normalize(Light.pos - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect(-l, norm);
    // float l_n = abs(dot(l, norm));
    float l_n = max(dot(l, norm), 0.0f);
    vec3 diffuse = Light.ld * Material.kd * l_n;
    vec3 ambient = Light.la * Material.ka;
    vec3 spec = vec3(0.0);
    if (l_n > 0.0f) {
        float r_v = max(dot(r, v), 0.0f);
        spec = Light.ls * Material.ks * pow(r_v, Material.shininess);
    }
    return clamp(ambient + diffuse + spec, vec3(0.0f), vec3(1.0f));
}

void main(void) {
    vec3 pos, normal;
    ViewSpace(pos, normal);
    vs_out.color = ADS(pos, normal);
    NDCSpace(gl_Position);
}
