#version 430 core

/* clang-format off */
in GS_FS {
    vec3 pos;
    vec3 normal;
    vec3 color;
}
fs_in;
/* clang-format off */

out vec4 color;

void main(void) {
    color = vec4(fs_in.color, 1.0f);
}
