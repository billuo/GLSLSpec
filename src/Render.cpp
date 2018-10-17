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
Model MyModel, MyAxis;

enum RasterizationMode { None = 0, Fill = GL_FILL, Line = GL_LINE, Point = GL_POINT };
bool Axes = true;
RasterizationMode FrontMode = Fill;
RasterizationMode BackMode = None;

std::unique_ptr<OpenGL::Program> ProgramTriangles, ProgramLines;
std::unique_ptr<OpenGL::ProgramInterface<OpenGL::Uniform>> UI;
std::unique_ptr<OpenGL::ProgramInterface<OpenGL::UniformBlock>> UBI;
std::unique_ptr<OpenGL::ProgramInterface<OpenGL::ProgramInput>> PII;
std::unique_ptr<OpenGL::ProgramInterface<OpenGL::ProgramOutput>> POI;

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
    ProgramTriangles.reset(new OpenGL::Program("Triangles"));
    ProgramTriangles->Attach(sphere_shaders).Link();
    // prepare program to draw axes
    std::vector<const OpenGL::Shader*> axes_shaders;
    for (auto&& file : { "shader.vert", "shader2.geom", "shader.frag" }) {
        axes_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, file));
    }
    ProgramLines.reset(new OpenGL::Program("Lines"));
    ProgramLines->Attach(axes_shaders).Link();
    // setup UBO
    UI = std::make_unique<OpenGL::ProgramInterface<OpenGL::Uniform>>(*ProgramTriangles);
    UBI = std::make_unique<OpenGL::ProgramInterface<OpenGL::UniformBlock>>(*ProgramTriangles);
    PII = std::make_unique<OpenGL::ProgramInterface<OpenGL::ProgramInput>>(*ProgramTriangles);
    POI = std::make_unique<OpenGL::ProgramInterface<OpenGL::ProgramOutput>>(*ProgramTriangles);
    CHECK_OPENGL();
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
    std::vector<glm::vec3> axes_vertices;
    axes_vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    axes_vertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    axes_vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    axes_vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    axes_vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    axes_vertices.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    // upload vertices
    {
        Mesh& mesh = MyModel.GetMesh();
        mesh.InitData(vertices.size());
        void* ptr = mesh.MapBufferVertex();
        glm::vec3* out = static_cast<glm::vec3*>(ptr);
        std::copy(vertices.begin(), vertices.end(), out);
        mesh.UnmapBufferVertex();
    }
    {
        Mesh& mesh = MyAxis.GetMesh();
        mesh.InitData(axes_vertices.size());
        void* ptr = mesh.MapBufferVertex();
        glm::vec3* out = static_cast<glm::vec3*>(ptr);
        std::copy(axes_vertices.begin(), axes_vertices.end(), out);
        mesh.UnmapBufferVertex();
    }
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
    static glm::vec3 fg_color(0.9f, 0.8f, 0.05f);
    static glm::vec3 axis_color(0.0f, 0.2f, 0.9f);
    // init
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));
    glClear(GL_DEPTH_BUFFER_BIT);
    auto ub_xform = UBI->find("Transformations");
    assert(ub_xform);
    CHECK_OPENGL();
    // draw axes
    glVertexAttrib3fv(2, glm::value_ptr(axis_color));
    auto u0 = ub_xform->find("World_Model");
    assert(u0);
    glNamedBufferSubData(UBO, u0->offset, OpenGL::TypeSize(u0->type), glm::value_ptr(MyAxis.GetTransform()));
    if (Render::Axes) {
        OpenGL::Program::Use(*ProgramLines);
        MyAxis.GetMesh().Draw(VAO, GL_LINES);
    }
    CHECK_OPENGL();
    // draw main object
    glVertexAttrib3fv(2, glm::value_ptr(fg_color));
    auto u1 = ub_xform->find("NDC_World");
    auto u2 = ub_xform->find("World_Model");
    assert(u1 && u2);
    glNamedBufferSubData(UBO, u1->offset, OpenGL::TypeSize(u1->type), glm::value_ptr(NDC_View * View_World));
    glNamedBufferSubData(UBO, u2->offset, OpenGL::TypeSize(u2->type), glm::value_ptr(MyModel.GetTransform()));
    OpenGL::Program::Use(*ProgramTriangles);
    RenderFront();
    RenderBack();
    // finish up
    glutSwapBuffers();
    if (GLint err = glGetError()) {
        exit(err);
    }
}

} // namespace Render
