#version 430 core

#ifdef BACKGROUND

#define PI 3.1415926

uniform ivec2 u_fbsize;
uniform ivec2 u_mpos;
uniform float u_time;

float plot(vec2 st, float pct) {
    float thickness = 0.01;
    return smoothstep(pct - thickness, pct, st.y) - smoothstep(pct, pct + thickness, st.y);
//    return step(pct - thickness/2, st.y) - step(pct + thickness/2, st.y);
}

float RaisedInvCos(float x) {
    return 1 - (cos(x * PI) + 1)/2;
}
float BWRaisedInvCos(float x) {
    return (4*pow(x,6) - 17*pow(x,4) + 22*pow(x,2))/9;
}

float quadraticThroughCursor(float x) {
    float eps = 0.0001;
    float a = 1.0 * u_mpos.x/u_fbsize.x, b = 1.0 * u_mpos.y/u_fbsize.y;
    a = clamp(a, eps, 1 - eps);
    b = clamp(b, 0.0f, 1.0f);
    float A = (1-b)/(1-a)-(b/a);
    float B = (A*(a*a)-b)/a;
    float y = A*(x*x) - B*x;
    return clamp(y, 0, 1);
}

float LogisticSigmoid(float x, float a) {
    float eps = 0.0001f;
    a = clamp(a, eps, 1-eps);
    a = (1/(1-a) - 1);
    float A = 1.0 / (1.0 + exp(-2*(x-0.5)*a));
    float B = 1.0 / (1.0 + exp(a));
    float C = 1.0 / (1.0 + exp(-a));
    return (A - B) / (C - B);
}

out vec4 o_color;

void main(void) {
    vec2 st = gl_FragCoord.xy / u_fbsize;
    float x = st.x;
    float fx = quadraticThroughCursor(x);
    vec3 color = vec3(fx);
//    color = mix(color, vec3(1.0f, 0.0f, 0.0f), plot(st, f(x)));
//    color = mix(color, vec3(0.0f, 1.0f, 0.0f), plot(st, g(x)));
    color = mix(color, vec3(0.0f, 0.0f, 1.0f), plot(st, fx));
    o_color = vec4(color, 1.0f);
}

#else

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 color;

out vec4 o_color;

void main(void) {
    o_color = vec4(color, 1.0f);
}

#endif
