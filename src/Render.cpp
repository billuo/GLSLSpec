/**
 * @file Render.cpp
 * @brief Take care of rendering
 * @author Zhen Luo 461652354@qq.com
 */
#include "Render.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "OpenGL/Introspection.hpp"

namespace Render {

namespace {
GLuint VAO, UBO;
Model MyModel;

enum RasterizationMode { None = 0, Fill = GL_FILL, Line = GL_LINE, Point = GL_POINT };
bool Axes = true;
RasterizationMode FrontMode = Fill;
RasterizationMode BackMode = None;

std::unique_ptr<OpenGL::Program> ProgramSphere, ProgramAxes;
std::unique_ptr<OpenGL::UniformInterface> UI_axes;
std::unique_ptr<OpenGL::UniformInterface> UI;
std::unique_ptr<OpenGL::UniformBlockInterface> UBI;
std::unique_ptr<OpenGL::ProgramInputInterface> PII;
std::unique_ptr<OpenGL::ProgramOutputInterface> POI;

glm::mat4 NDC_View = glm::identity<glm::mat4>();
glm::mat4 View_World = glm::identity<glm::mat4>();
} // namespace

//
// Misc
//

static inline RasterizationMode NextMode(RasterizationMode mode) {
    switch (mode) {
    case None: return Fill;
    case Fill: return Line;
    case Line: return Point;
    case Point: return None;
    default: return None;
    }
}

void ToggleAxes() { Axes = !Axes; }
void SwitchRasterizationMode(Side side) {
    if (side == Side::Back) {
        BackMode = NextMode(BackMode);
    } else {
        FrontMode = NextMode(FrontMode);
    }
}
void SetProjectionMatrix(const glm::mat4& mat) { NDC_View = mat; }
void SetViewMatrix(const glm::mat4& mat) { View_World = mat; }

float Shininess = 1.0f;

//
// Init
//

static void InitMisc() {
    glDebugMessageCallback(MyDebugMessageCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glPointSize(3.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

static void InitShaderProgram() {
    const std::string shader_dir("../shaders/");
    // prepare main program
    std::vector<const OpenGL::Shader*> sphere_shaders;
    for (auto&& file : { "shader.vert", "shader.geom", "shader.frag" }) {
        sphere_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, file));
    }
    ProgramSphere.reset(new OpenGL::Program("Sphere"));
    ProgramSphere->Attach(sphere_shaders).Link();
    // prepare program to draw axes
    std::vector<const OpenGL::Shader*> axes_shaders;
    for (auto&& file : { "axes.vert", "axes.frag" }) {
        axes_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, file));
    }
    ProgramAxes.reset(new OpenGL::Program("Axes"));
    ProgramAxes->Attach(axes_shaders).Link();
    // setup UBO
    UI_axes = std::make_unique<OpenGL::UniformInterface>(*ProgramAxes);
    UI = std::make_unique<OpenGL::UniformInterface>(*ProgramSphere);
    UBI = std::make_unique<OpenGL::UniformBlockInterface>(*ProgramSphere);
    PII = std::make_unique<OpenGL::ProgramInputInterface>(*ProgramSphere);
    POI = std::make_unique<OpenGL::ProgramOutputInterface>(*ProgramSphere);
    CHECK_OPENGL();
    UI_axes->dump();
    UI->dump();
    UBI->dump();
    PII->dump();
    POI->dump();
    auto ub_xform = UBI->find("Transformations");
    assert(ub_xform);
    glCreateBuffers(1, &UBO);
    glNamedBufferStorage(UBO, ub_xform->size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, ub_xform->binding, UBO);
}

static void SphereVetices(float radius, size_t n_slices, size_t n_layers, std::vector<glm::vec3>& vertices,
                          std::vector<glm::vec3>& normals) {
    const float dtheta = 2 * Pi / n_slices;
    const float dphi = Pi / n_layers;
    float r1 = 0.0f;
    float r2 = glm::sin(dphi);
    float y1 = 1.0f;
    float y2 = glm::cos(dphi);
    for (size_t layer = 0; layer <= n_layers; ++layer) {
        float theta1 = 0.0f;
        float theta2 = theta1 + dtheta;
        for (size_t slice = 0; slice < n_slices; ++slice) {
            vertices.push_back(glm::vec3(glm::cos(theta1) * r2, y2, glm::sin(theta1) * r2));
            vertices.push_back(glm::vec3(glm::cos(theta1) * r1, y1, glm::sin(theta1) * r1));
            vertices.push_back(glm::vec3(glm::cos(theta2) * r2, y2, glm::sin(theta2) * r2));
            vertices.push_back(glm::vec3(glm::cos(theta2) * r2, y2, glm::sin(theta2) * r2));
            vertices.push_back(glm::vec3(glm::cos(theta1) * r1, y1, glm::sin(theta1) * r1));
            vertices.push_back(glm::vec3(glm::cos(theta2) * r1, y1, glm::sin(theta2) * r1));
            theta1 = theta2;
            theta2 = theta1 + dtheta;
        }
        r1 = r2;
        r2 = glm::sin((layer + 1) * dphi);
        y1 = y2;
        y2 = glm::cos((layer + 1) * dphi);
    }
    normals = vertices;
    const size_t n_vertices = vertices.size();
    for (size_t i = 0; i < n_vertices; ++i) {
        vertices[i] *= radius;
    }
}

void Init() {
    InitMisc();
    // create VAO
    glCreateVertexArrays(1, &VAO);
    // prepare vertices
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    SphereVetices(0.8f, 120, 60, vertices, normals);
    /// @TODO use 'normals'
    // upload vertices
    Mesh& mesh = MyModel.GetMesh();
    mesh.InitData(vertices.size());
    void* ptr = mesh.MapBufferVertex();
    glm::vec3* out = static_cast<glm::vec3*>(ptr);
    std::copy(vertices.begin(), vertices.end(), out);
    mesh.UnmapBufferVertex();
    InitShaderProgram();
}

//
// Render
//

static void RenderFront() {
    if (FrontMode != None) {
        glFrontFace(GL_CCW);
        glPolygonMode(GL_FRONT_AND_BACK, FrontMode);
        MyModel.GetMesh().Draw(VAO, GL_TRIANGLES);
        CHECK_OPENGL();
    }
}
static void RenderBack() {
    if (BackMode != None) {
        glPolygonMode(GL_FRONT_AND_BACK, BackMode);
        glFrontFace(GL_CW);
        MyModel.GetMesh().Draw(VAO, GL_TRIANGLES);
        CHECK_OPENGL();
    }
}

void Render() {
    static glm::vec4 bg_color(0.9f, 0.9f, 0.9f, 1.0f);
    //
    // init
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));
    glClear(GL_DEPTH_BUFFER_BIT);
    CHECK_OPENGL();
    //
    // draw axes
    auto u_world_axes = UI_axes->find("NDC_World");
    assert(u_world_axes);
    if (Render::Axes) {
        OpenGL::Program::Use(*ProgramAxes);
        glUniformMatrix4fv(u_world_axes->location, 1, GL_FALSE, glm::value_ptr(NDC_View * View_World));
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 6);
    }
    CHECK_OPENGL();
    //
    // draw main object
    OpenGL::Program::Use(*ProgramSphere);
    // light
    // auto u_lpos = UI->find("Light.pos");
    auto u_la = UI->find("Light.la");
    auto u_ld = UI->find("Light.ld");
    auto u_ls = UI->find("Light.ls");
    // glUniform3fv(u_lpos->location, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
    glUniform3fv(u_la->location, 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.01f)));
    glUniform3fv(u_ld->location, 1, glm::value_ptr(glm::vec3(0.9f, 0.8f, 0.03f)));
    glUniform3fv(u_ls->location, 1, glm::value_ptr(glm::vec3(0.9f, 0.8f, 0.03f)));
    // material
    auto u_ka = UI->find("Material.ka");
    auto u_kd = UI->find("Material.kd");
    auto u_ks = UI->find("Material.ks");
    auto u_shiniess = UI->find("Material.shininess");
    glUniform3fv(u_ka->location, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 1.0f)));
    glUniform3fv(u_kd->location, 1, glm::value_ptr(glm::vec3(0.9f)));
    glUniform3fv(u_ks->location, 1, glm::value_ptr(glm::vec3(0.9f)));
    glUniform1f(u_shiniess->location, Shininess);
    auto ub_xform = UBI->find("Transformations");
    auto u_model = ub_xform->find("View_Model");
    auto u_normal = ub_xform->find("NormalMatrix");
    auto u_view = ub_xform->find("NDC_View");
    auto u_mvp = ub_xform->find("NDC_Model");
    // transformations
    glm::mat4 View_Model = View_World * MyModel.GetTransform();
    glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(View_Model))); // inverse transpose matrix
    glNamedBufferSubData(UBO, u_model->offset, OpenGL::TypeSize(u_model->type), glm::value_ptr(View_Model));
    for (glm::length_t i = 0; i < NormalMatrix.length(); ++i) {
        glNamedBufferSubData(UBO, u_normal->offset + u_normal->mstride * i, 3 * sizeof(GLfloat),
                             glm::value_ptr(NormalMatrix[i]));
    }
    glNamedBufferSubData(UBO, u_view->offset, OpenGL::TypeSize(u_view->type), glm::value_ptr(NDC_View));
    glNamedBufferSubData(UBO, u_mvp->offset, OpenGL::TypeSize(u_mvp->type), glm::value_ptr(NDC_View * View_Model));
    // render
    RenderFront();
    RenderBack();
    //
    // finish up
    glutSwapBuffers();
    if (GLint err = glGetError()) {
        exit(err);
    }
}

} // namespace Render
