#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_GS {
    vec3 pos;
    vec3 color;
} gs_in[];

out GS_FS {
    vec3 pos;
    vec3 color;
} gs_out;

subroutine vec3 colorOfIndex(int i);
layout(index = 1)
subroutine (colorOfIndex)
vec3 smoothColor(int i) {
    return gs_in[i].color;
}

layout(index = 2)
subroutine (colorOfIndex)
vec3 flatColor(int i) {
    return (gs_in[0].color + gs_in[1].color + gs_in[2].color) / 3;
}

subroutine uniform colorOfIndex getColor;

void main(void) {
    for (int i = 0; i < gl_in.length(); ++i) {
        gl_Position = gl_in[i].gl_Position;
        gs_out.pos = gs_in[i].pos;
        gs_out.color = getColor(i);
        EmitVertex();
    }
    EndPrimitive();
}
