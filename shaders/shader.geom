#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

/* clang-format off */
in VS_GS {
    vec3 pos;
    vec3 color;
} gs_in[];

out GS_FS {
    vec3 pos;
    flat vec3 color;
} gs_out;
/* clang-format on */

vec3 GetColor(int i) { return (gs_in[0].color + gs_in[1].color + gs_in[2].color) / gs_in.length(); }

void main(void) {
    for (int i = 0; i < gl_in.length(); ++i) {
        gl_Position = gl_in[i].gl_Position;
        gs_out.pos = gs_in[i].pos;
        gs_out.color = GetColor(i);
        EmitVertex();
    }
    EndPrimitive();
}
