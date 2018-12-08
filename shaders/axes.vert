
#version 430 core

// uniform mat4 NDC_World;

out gl_PerVertex {
    vec4 gl_Position;
};

out vec3 color;

void main(void) {
    const vec3[] vertices = vec3[](vec3(0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
    const vec3[] colors = vec3[](vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
    if ((gl_VertexID & 1) == 1) {
        // gl_Position = NDC_World * vec4(vertices[gl_VertexID / 2 + 1], 1.0f);
        gl_Position = vec4(vertices[gl_VertexID / 2 + 1], 1.0f);
    } else {
        // gl_Position = NDC_World * vec4(vec3(0.0f), 1.0f);
        gl_Position = vec4(vec3(0.0f), 1.0f);
    }
    color = colors[gl_VertexID / 2];
}
