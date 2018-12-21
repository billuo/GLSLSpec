#version 430 core

#if defined(POSTPROCESS)

uniform sampler2D u_scene;
uniform sampler2D u_depth;

uniform vec3 u_camera;
uniform float u_time;

uniform mat4 VM;

in vec2 v_position;
in vec2 p_texcoord;

out vec4 o_color;

// intersection
float I(float a, float b) {
    return max(a, b);
}

// union
float U(float a, float b) {
    return min(a, b);
}

// difference
float D(float a, float b) {
    return max(a, -b);
}

// symmetric difference
float SD(float a, float b) {
    return U(D(a, b), D(b, a));
}

// complement
float C(float a) {
    return -a;
}

// primitive shapes

float Sphere(vec3 p, vec3 c, float r)
{
    return length(p - c) - r;
}

float distance_closest(vec3 p) {
    return D(Sphere(p, vec3(0.0), 1.0), Sphere(p, vec3(0.0, 1.0, 0.0), 0.5));
}

vec3 estimate_normal(vec3 p) {
    const vec2 epsilon = vec2(0.001, 0.0);
    float dx = distance_closest(p + epsilon.xyy) - distance_closest(p - epsilon.xyy);
    float dy = distance_closest(p + epsilon.yxy) - distance_closest(p - epsilon.yxy);
    float dz = distance_closest(p + epsilon.yyx) - distance_closest(p - epsilon.yyx);
    return normalize(vec3(dx, dy, dz));
}

/// @param ro Origin of ray
/// @param rd Direction of ray
vec3 ray_march(vec3 ro, vec3 rd) {
    float total = 0.0;
    const int MaxSteps = 32;
    const float MinHitDistance = 0.001;
    const float MaxTraceDistance = 120.0;

    for (int i = 0; i < MaxSteps; ++i) {
        vec3 current = ro + total * rd;
        float d_closest = distance_closest(current);
        if (d_closest < MinHitDistance) {
            // Hit
            vec3 lpos = vec3(5 * sin(u_time), 5.0, 3 * cos(u_time));
            vec3 n = estimate_normal(current);
            vec3 l = normalize(lpos - current);
            return vec3(1.0, 0.0, 0.0) * max(0.0, dot(n, l));
        }
        if (d_closest > MaxTraceDistance) {
            // Totally missed
            break;
        }
        total += d_closest;
    }
    return vec3(0.2);
}


void main() {
//    vec3 view_dir = vec3(v_position, 1.0 / tan(radians(60.0/2.0)));
    vec3 view_dir = vec3(v_position, -1.0);
    mat3 view_to_world = transpose(mat3(VM));
    vec3 world_dir = view_to_world * view_dir;
    vec3 color = ray_march(u_camera, world_dir);
    o_color = vec4(color, 1.0);
}

#endif
