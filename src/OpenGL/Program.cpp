#include "OpenGL/Program.hpp"
#include "Debug.hpp"
#include <algorithm>
#include <utility>

namespace OpenGL {

void Program::Attach(const Shader& shader) {
    if (aux_CheckInitialized(true)) {
        if (shader.Initialized()) {
            glAttachShader(Name(), shader.Name());
        }
    }
}

void Program::Detach(const Shader& shader) {
    if (aux_CheckInitialized(true)) {
        if (shader.Initialized()) {
            glDetachShader(Name(), shader.Name());
        }
    }
}

void Program::Link() {
    if (aux_CheckInitialized(true)) {
        glLinkProgram(Name());
        if (aux_Get(GL_LINK_STATUS) == GL_FALSE) {
            DEBUG("Shader program linking failed:\n%s", GetInfoLog().c_str());
        }
    }
}

void Program::Use() {
    if (aux_CheckInitialized(true)) {
        glUseProgram(Name());
    }
}

GLint Program::GetInterface(GLenum program_interface, GLenum pname) const {
    GLint result = -1;
    if (aux_CheckInitialized(true)) {
        glGetProgramInterfaceiv(Name(), program_interface, pname, &result);
    }
    return result;
}

void Program::List(GLenum interface) const {
    printf("Active %s in program<name=%u>:\n", InterfaceString(interface), Name());
    const GLint n_attributes = GetInterface(interface, GL_ACTIVE_RESOURCES);
    const GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
    const size_t N = countof(properties);
    GLint results[N];
    std::vector<Resource> attributes(n_attributes);
    GLint max_name_length = 0;
    for (GLint i = 0; i < n_attributes; ++i) {
        glGetProgramResourceiv(Name(), interface, i, N, properties, N, NULL, results);
        max_name_length = std::max(max_name_length, results[0]);
        attributes[i].type = results[1];
        attributes[i].index = results[2];
    }
    GLchar* name = new GLchar[max_name_length + 1];
    for (GLint i = 0; i < n_attributes; ++i) {
        glGetProgramResourceName(Name(), interface, i, max_name_length + 1, NULL, name);
        printf("[%2d] %s %s\n", attributes[i].index, TypeString(attributes[i].type), name);
    }
    delete[] name;
}

const Program::UniformBlock* Program::GetUniformBlock(const GLchar* name) const {
    UniformBlock* ret = new UniformBlock;
    // get index and size of the uniform block
    GLint n_uniforms;
    ret->index = glGetUniformBlockIndex(Name(), name);
    glGetActiveUniformBlockiv(Name(), ret->index, GL_UNIFORM_BLOCK_DATA_SIZE, &(ret->size));
    glGetActiveUniformBlockiv(Name(), ret->index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &n_uniforms);
    ret->uniforms.resize(n_uniforms);
    CHECK_OPENGL();
    // get indices, names and offsets of the uniforms in the uniform block
    GLint* uniform_indices = new GLint[n_uniforms];
    glGetActiveUniformBlockiv(
            Name(), ret->index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
            uniform_indices); // XXX glGetProgramResourceiv return -1 for GL_LOCATION of uniforms in uniform block.
    for (GLint i = 0; i < n_uniforms; ++i) {
        ret->uniforms[i].index = uniform_indices[i];
    }
    delete[] uniform_indices;
    const GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_OFFSET };
    GLint results[countof(properties)];
    const size_t N = countof(properties);
    for (GLint i = 0; i < n_uniforms; ++i) {
        glGetProgramResourceiv(Name(), GL_UNIFORM, i, N, properties, N, NULL, results);
        GLchar* uniform_name = new GLchar[results[0] + 1];
        glGetProgramResourceName(Name(), GL_UNIFORM, i, results[0] + 1, NULL, uniform_name);
        ret->uniforms[i].type = results[1];
        ret->uniforms[i].offset = results[2];
        ret->uniforms[i].name = uniform_name;
        delete[] uniform_name;
    }
    ret->name.assign(name);
    return ret;
}

std::string Program::GetInfoLog() const {
    std::string ret;
    if (aux_CheckInitialized(true)) {
        GLint length = aux_Get(GL_INFO_LOG_LENGTH);
        assert(length >= 0);
        ret.resize(length);
        glGetProgramInfoLog(Name(), length, NULL, &ret[0]);
    }
    return ret;
}

GLint Program::aux_Get(GLenum pname) const {
    GLint result = -1;
    if (aux_CheckInitialized(true)) {
        glGetProgramiv(Name(), pname, &result);
    }
    return result;
}

GLint Program::aux_GetStage(GLenum stage, GLenum pname) const {
    GLint result = -1;
    if (aux_CheckInitialized(true)) {
        glGetProgramStageiv(Name(), stage, pname, &result);
    }
    return result;
}

} // namespace OpenGL
