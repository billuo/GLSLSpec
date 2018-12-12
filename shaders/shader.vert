#version 430 core

out gl_PerVertex {
    vec4 gl_Position;
};

out VS_GS {
    vec3 pos;
    vec3 color;
} vs_out;

in vec3 v_position;
in vec3 v_normal;

struct light_t {
    vec3 pos; // light position [eye coord.]
    vec3 la;  // ambient light intensity
    vec3 ld;  // diffuse light intensity
    vec3 ls;  // specular light intensity
};

uniform light_t L;

struct material_t {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
};
uniform material_t M;

uniform mat4 PV;
uniform mat4 VM;
uniform mat4 PVM;
uniform mat3 NM;

void ViewSpace(out vec3 position, out vec3 normal) {
    position = (VM * vec4(v_position, 1.0f)).xyz;
    normal = normalize(NM * v_normal);
}

void NDCSpace(out vec4 position) { position = PVM * vec4(v_position, 1.0f); }

vec3 ADS(vec3 pos, vec3 norm) {
    vec3 l = normalize(L.pos - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect(-l, norm);
    // float l_n = abs(dot(l, norm));
    float l_n = max(dot(l, norm), 0.0f);
    vec3 diffuse = L.ld * M.kd * l_n;
    vec3 ambient = L.la * M.ka;
    vec3 spec = vec3(0.0);
    if (l_n > 0.0f) {
        float r_v = max(dot(r, v), 0.0f);
        spec = L.ls * M.ks * pow(r_v, M.shininess);
    }
    return clamp(ambient + diffuse + spec, vec3(0.0f), vec3(1.0f));
}

void main(void) {
    vec3 pos, normal;
    ViewSpace(pos, normal);
    vs_out.color = ADS(pos, normal);
    NDCSpace(gl_Position);
}
