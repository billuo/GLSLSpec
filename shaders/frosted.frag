#version 430 core

#if defined(POSTPROCESS)

uniform sampler2D u_scene;
uniform ivec2 u_fbsize;
uniform float u_time;

in vec2 v_position;
in vec2 v_texcoord;

out vec4 o_color;

// #include "noise.glsl"
vec2 mod289(vec2 x)
{return x - floor(x/289.0)*289.0;}

vec3 mod289(vec3 x)
{return x - floor(x/289.0)*289.0;}

vec3 permute(vec3 x)
{return mod289((x*34.0)+1.0*x);}

/// 1D simplex noise function
/// came from the book 'OpenGL Insights'
float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187,  //(3.0-sqrt(3.0))/6.0
                        0.366025403784439,  //0.5*(sqrt(3.0)-1.0)
                       -0.577350269189626,  //-1.0+2.0*C.x
                        0.024390243902439); //1.0/41.0
    vec2 i = floor(v + dot(v,C.yy));
    vec2 x0 = v - i + dot(i,C.xx);
    vec2 i1 = (x0.x>x0.y) ? vec2(1.0,0.0):vec2(0.0,1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod289(i);
    vec3 p = permute(permute(i.y + vec3(0.0,i1.y,1.0)) + i.x + vec3(0.0,i1.x,1.0));
    vec3 m = max(0.5 - vec3(dot(x0,x0),dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)),0.0);
    m = m*m;
    m = m*m;
    vec3 x = 2.0*fract(p*C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 a0 = x - floor(x + 0.5);
    m *= 1.79284291400159 - 0.85373472095314*(a0*a0 + h*h);
    vec3 g;
    g.x = a0.x*x0.x + h.x*x0.y;
    g.yz = a0.yz*x12.xz + h.yz*x12.yw;
    return 130.0*dot(m,g);
}

vec3 spline(float x, vec3 control[9]) {
    float i;
    float w = modf(8*x, i);
    int index = int(min(i, 7.0));
    return mix(control[index], control[index+1], w);
}

vec3 frosted(vec2 uv) {
    const vec2 pixel = vec2(2.5);
    const float freq = 0.05;
    vec4 delta = vec4(pixel / u_fbsize, 0.0, 0.0);
    delta.w = -delta.x; // delta = {dx, dy, 0, -dx}
    vec3 c00 = texture(u_scene, uv - delta.xy).rgb;
    vec3 c01 = texture(u_scene, uv - delta.zy).rgb;
    vec3 c02 = texture(u_scene, uv - delta.wy).rgb;
    vec3 c10 = texture(u_scene, uv - delta.xz).rgb;
    vec3 c11 = texture(u_scene, uv).rgb;
    vec3 c12 = texture(u_scene, uv + delta.xz).rgb;
    vec3 c20 = texture(u_scene, uv + delta.wy).rgb;
    vec3 c21 = texture(u_scene, uv + delta.zy).rgb;
    vec3 c22 = texture(u_scene, uv + delta.xy).rgb;
    return spline((snoise(vec2(freq*u_time, snoise(uv*25)))+1.0)/2.0, vec3[9](c00, c01, c02, c10, c11, c12, c20, c21, c22));
}

void main() {
    o_color = vec4(mix(frosted(v_texcoord), texture(u_scene, v_texcoord).rgb, step(1.0, v_texcoord.x)), 1.0);
}

#endif
