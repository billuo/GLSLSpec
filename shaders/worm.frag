#version 430 core

// Created by inigo quilez - iq/2014
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0

// ported to GLSLSpec

uniform ivec2 u_fbsize;
uniform float u_time;
uniform vec3 u_camera;
uniform mat4 VM;

in vec2 v_position;
in vec2 v_texcoord;

out vec4 o_color;

float hash(vec2 p)
{ return fract(sin(1.0+dot(p,vec2(127.1,311.7)))*43758.545); }

vec2 sin_cos(float x)
{ return vec2(sin(x), cos(x)); }

vec3 opU(vec3 d1, vec3 d2)
{
    const float k = 0.03f;
    vec3 h = clamp(0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);
    return mix(d2, d1, h) - k*h*(1.0-h);
}

vec2 sdSegment(vec3 p, vec3 a, vec3 b)
{
    vec3 ap = p - a, ab = b - a;
	float h = clamp(dot(ap,ab)/dot(ab,ab), 0.0, 1.0); // propotion of p projected to ab, clamped to [0, 1].
	vec3 v = ap - ab*h;
	return vec2(length(v), h);
}

vec3 map(vec3 p)
{
    vec2 id = floor((p.xz+1.0)/2.0);
    float ph = hash(id+113);
    float ve = hash(id);

    p.xz = mod(p.xz+1.0, 2.0) - 1.0; // repeat every 2 unit
    p.xz += 0.5*cos(2.0*ve*u_time + (p.y+ph)*vec2(0.53,0.32) - vec2(1.57,0.0));

    vec3 phase = vec3(0.0, 3.14/3, 6.28/3)+p.yyy;

    vec3 p1 = p; p1.xz += 0.15*sin_cos(phase.x);
    vec3 p2 = p; p2.xz += 0.15*sin_cos(phase.y);
    vec3 p3 = p; p3.xz += 0.15*sin_cos(phase.z);

    const vec3 segBegin = vec3(0.0, 50.0, 0.0);
    const vec3 segEnd = vec3(0.0, -50.0, 0.0);
    vec2 h1 = sdSegment(p1, segBegin, segEnd);
    vec2 h2 = sdSegment(p2, segBegin, segEnd);
    vec2 h3 = sdSegment(p3, segBegin, segEnd);

    vec3 d1 = vec3(h1.x-0.15*(0.8+0.2*sin(200.0*h1.y+2*u_time)), ve + 0.000, h1.y);
    vec3 d2 = vec3(h2.x-0.15*(0.8+0.2*sin(200.0*h2.y+3*u_time)), ve + 0.015, h2.y);
    vec3 d3 = vec3(h3.x-0.15*(0.8+0.2*sin(200.0*h3.y+4*u_time)), ve + 0.030, h3.y);
    return opU(opU(d1, d2), d3);
}

// TODO px?
vec3 intersect(in vec3 ro, in vec3 rd, in float px, const float MaxDistance)
{
    vec3 res = vec3(-1.0);
    float t = 0.0;
    for(int i = 0; i < 256; i++) {
	    vec3 h = map(ro + t * rd);
        res = vec3(t, h.yz);
        if(h.x < (px * t) || t > MaxDistance) {
            break;
        }
        t += min(h.x, 0.5)*0.5; // be conservative to converge better
    }
	return res;
}

vec3 estimate_normal(in vec3 pos)
{
    const vec2 e = vec2(1.0,-1.0)/512.0;
    return normalize(e.xyy*map(pos + e.xyy).x +
					 e.yyx*map(pos + e.yyx).x +
					 e.yxy*map(pos + e.yxy).x +
					 e.xxx*map(pos + e.xxx).x);
}

float estimate_occlusion(in vec3 pos, in vec3 nor)
{
    const float h = 0.1;
	float ao = 0.0;
    for(int i=0; i < 8; i++)
    {
        vec3 dir = sin(float(i)*vec3(1.0,7.13,13.71)+vec3(0.0,2.0,4.0));
        dir = dir + 2.0*nor*max(0.0,-dot(nor,dir));
        float d = map(pos + h*dir).x;
        ao += h-d;
    }
    return clamp(1.0 - 0.7*ao, 0.0, 1.0);
}

vec3 render(in vec3 ro, in vec3 rd, in float px)
{
    vec3 col = vec3(0.0);

    const float MaxDistance = 32.0;
    vec3 res = intersect(ro, rd, px, MaxDistance);
    if(res.x < MaxDistance) {
        vec3 current = ro + res.x*rd;
        vec3 normal = estimate_normal(current);
        float occ = estimate_occlusion(current, normal);

        /// give color based on id
        col += 0.5 + 0.5*cos(res.y*31.4 + vec3(0.2,4.4,4.0));
        /// shed light from above
        col *= 0.5 + 1.5*normal.y;
        /// add highlight on border
        col += clamp(1.0+dot(rd,normal),0.0,1.0);

        /// circular pattern
        float u = 800.0*res.z - sin(res.y)*u_time;
        col *= 0.95 + 0.05*cos(u + 3.1416*cos(1.5*u + 3.1416*cos(3.0*u)) + vec3(0.0,1.0,2.0));

        /// tune to orange
        col *= vec3(1.5,1.0,0.7);
        /// TODO ?
        col *= occ;

        /// moving high light
        float fl = mod((0.5+cos(2.0+res.y*47.0))*u_time*2 + res.y*7.0, 4.0)/4.0;
        col *= 2.5 - 1.5*smoothstep(0.02,0.04,abs(res.z-fl));
        /// MOD moving shadow in reverse direction
//         float fl_rev = mod(-(0.5+cos(2.0+res.y*93.0))*u_time*3 + res.y*3.0, 4.0)/4.0;
//         col *= 0.5 + 0.5*smoothstep(0.02,0.04,abs(res.z-fl_rev));

        /// attenuation
        col *= exp(-res.x/16.0); // all along the way
        col *= 1.0 - smoothstep(16.0, 64.0, res.x); // soft cap in the distance
    }

    return pow(col, vec3(0.5,1.0,1.0));
}

void main()
{
    const float ar = u_fbsize.x * 1.0 / u_fbsize.y;
	vec2 p = v_position;
    vec2 q = v_texcoord;
	p.x *= ar;

    const float fl = ar;
    vec3 view_dir = vec3(p, -1.0);
    mat3 view_to_world = transpose(mat3(VM));
    view_to_world[2] *= fl;
    vec3 rd = view_to_world * view_dir;

    vec3 col = render(u_camera, rd, 1.0/(u_fbsize.y*fl));

    col *= pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1);

	o_color = vec4(col, 1.0);
}

