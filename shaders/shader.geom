#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

/* clang-format off */
in VS_GS {
    vec3 pos;
    vec3 normal;
    vec3 color;
} gs_in[];

out GS_FS {
    vec3 pos;
    vec3 normal;
    vec3 color;
} gs_out;
/* clang-format on */

void main(void) {
    for (int i = 0; i < gl_in.length(); ++i) {
        gl_Position = gl_in[i].gl_Position;
        gs_out.pos = gs_in[i].pos;
        gs_out.normal = gs_in[i].normal;
        gs_out.color = gs_in[i].color;
        EmitVertex();
    }
    EndPrimitive();
}
