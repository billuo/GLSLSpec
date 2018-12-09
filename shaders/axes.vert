
#version 430 core

uniform mat4 MVP;

out gl_PerVertex {
    vec4 gl_Position;
};

out vec3 color;

vec4 ndc_world(vec3 pos){
    mat4 scale = mat4(1.0f);
    scale[3][3] = 1.0f;
    return  MVP * scale * vec4(pos, 1.0f);
}

void main(void) {
    const vec3[] vertices = vec3[](vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
    const vec3[] colors = vec3[](vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
    if ((gl_VertexID & 1) == 1) {
         gl_Position = ndc_world(vertices[gl_VertexID / 2]);
    } else {
         gl_Position =  ndc_world(vec3(0.0f));
    }
    color = colors[gl_VertexID / 2];
}
