#version 430

out gl_PerVertex {
    vec4 gl_Position;
};

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;

out vec3 o_position;
out vec3 o_normal;
out vec2 o_texcoord;
out vec3 o_color;

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

uniform sampler2D u_sampler_1;

uniform mat4 PV;
uniform mat4 VM;
uniform mat4 PVM;
uniform mat3 NM;

uniform float u_time;

void ViewSpace(out vec3 position, out vec3 normal) {
    position = (VM * vec4(v_position, 1.0f)).xyz;
    normal = normalize(NM * v_normal);
}

vec3 ADS(vec3 pos, vec3 norm) {
    vec3 lpos = vec3(L.pos.x * sin(u_time), L.pos.y, L.pos.z * cos(u_time));
    vec3 l = normalize(lpos - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect(-l, norm);
    // float l_n = abs(dot(l, norm));
    float l_n = max(dot(l, norm), 0.0f);
    // float l_n = dot(l, norm);
    // if (l_n < 0.0f) return vec3(1.0f, 0.0f, 1.0f);
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
    ViewSpace(o_position, o_normal);
    o_color = ADS(o_position, o_normal);
    // o_color = mix(o_color, texture(u_sampler_1, v_texcoord).xyz, vec3(1.0f, 0.0f, 1.0f));
    // o_color = mix(o_color, texture(u_sampler_1, v_texcoord).xyz, vec3(0.3f, 0.7f, 0.3f));
    gl_Position = PVM * vec4(v_position, 1.0f);
    o_texcoord = v_texcoord;
}
