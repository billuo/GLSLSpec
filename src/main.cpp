/**
 * @file main.cpp
 * @brief Program entry point.
 * @author Zhen Luo 461652354@qq.com
 */
#include "Debug.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "OpenGL/Introspection.hpp"
#include <algorithm>
#include <bitset>
#include <fstream>
#include <vector>

namespace {

static GLuint VAO;

static std::unique_ptr<OpenGL::Program> ProgramTriangles, ProgramLines;
static std::unique_ptr<OpenGL::ProgramInterface<OpenGL::Uniform>> UI;
static std::unique_ptr<OpenGL::ProgramInterface<OpenGL::UniformBlock>> UBI;
static Model MyModel, MyAxis;
static GLuint UBO;

static glm::mat4 NDC_View = glm::identity<glm::mat4>();
static glm::mat4 View_World = glm::identity<glm::mat4>();
static glm::vec3 EyePos = glm::vec3(0.0f, 0.0f, 2.0f);
static glm::vec3 LookDir = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

} // namespace

namespace Draw {

enum RasterizationMode { None = 0, Fill = GL_FILL, Line = GL_LINE, Point = GL_POINT };
static inline RasterizationMode NextMode(RasterizationMode mode) {
    switch (mode) {
    case None: return Fill;
    case Fill: return Line;
    case Line: return Point;
    case Point: return None;
    default: DEBUG("Bad RasterizationMode"); return None;
    }
}
static RasterizationMode Front = Fill;
static RasterizationMode Back = None;
static bool Axes = true;

} // namespace Draw

namespace Input {

static std::bitset<128> KeyPressed;

static struct {
    float horizontal;
    float vertical;
} ViewAngle = { 0.0f, 0.0f };

static struct {
    int x;
    int y;
} LastGrabbing;

static void OnMotion(int x, int y) {
    x -= LastGrabbing.x;
    y -= LastGrabbing.y;
    LastGrabbing.x += x;
    LastGrabbing.y += y;
    y = -y; // XXX in glut, window coordinate originates at top left.
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    const float x_degrees(180.0f / w * x);
    const float y_degrees(180.0f / h * y);
    ViewAngle.horizontal += x_degrees;
    ViewAngle.vertical += y_degrees;
    if (ViewAngle.horizontal > 180.0f || ViewAngle.horizontal <= -180.0f) {
        ViewAngle.horizontal = std::fmod(ViewAngle.horizontal + 180.0f, 360.0f) - 180.0f;
    }
    const float y_mag_max = 89.0f;
    if (ViewAngle.vertical > y_mag_max) {
        ViewAngle.vertical = y_mag_max;
    } else if (ViewAngle.vertical < -y_mag_max) {
        ViewAngle.vertical = -y_mag_max;
    }
    LookDir.x = glm::cos(RadianOfDegree(ViewAngle.vertical)) * glm::sin(RadianOfDegree(ViewAngle.horizontal));
    LookDir.y = glm::sin(RadianOfDegree(ViewAngle.vertical));
    LookDir.z = glm::cos(RadianOfDegree(ViewAngle.vertical)) * -glm::cos(RadianOfDegree(ViewAngle.horizontal)); // XXX
}

static void OnMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            LastGrabbing.x = x;
            LastGrabbing.y = y;
        } else if (state == GLUT_UP) {
            /// @TODO
        }
    }
}

static void OnKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '\\': Draw::Axes = !Draw::Axes; break;
    case '[': Draw::Front = Draw::NextMode(Draw::Front); break;
    case ']': Draw::Back = Draw::NextMode(Draw::Back); break;
    case 'w':
    case 'a':
    case 's':
    case 'd': KeyPressed[key] = true; break;
    case 27: glutLeaveMainLoop(); break;
    default: break;
    }
}

static void OnKeyboardUp(unsigned char key, int x, int y) {
    /// @TODO
    switch (key) {
    case 'w':
    case 'a':
    case 's':
    case 'd': KeyPressed[key] = false; break;
    default: break;
    }
}

static void OnSpecial(int key, int x, int y) {
    /// @TODO
    switch (key) {
    default: break;
    }
}

} // namespace Input

void MyDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
                            const GLchar* message, const void* user);

static void InitShaderProgram() {
    const std::string shader_dir("./shaders/");
    // prepare main program
    std::vector<const OpenGL::Shader*> sphere_shaders;
    sphere_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, "shader.vert"));
    sphere_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, "shader.geom"));
    sphere_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, "shader.frag"));
    ProgramTriangles.reset(new OpenGL::Program);
    ProgramTriangles->Attach(sphere_shaders);
    ProgramTriangles->Link();
    // prepare program to draw axes
    std::vector<const OpenGL::Shader*> axes_shaders;
    axes_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, "shader.vert"));
    axes_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, "shader2.geom"));
    axes_shaders.push_back(OpenGL::Shader::CompileFrom(shader_dir, "shader.frag"));
    ProgramLines.reset(new OpenGL::Program);
    ProgramLines->Attach(axes_shaders);
    ProgramLines->Link();
    // setup UBO
    UI = std::make_unique<OpenGL::ProgramInterface<OpenGL::Uniform>>(*ProgramTriangles);
    UBI = std::make_unique<OpenGL::ProgramInterface<OpenGL::UniformBlock>>(*ProgramTriangles);
    auto ub_xform = UBI->find("Transformations");
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

static void InitDraw() {
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
}

static void InitMisc() {
    // printf("Max number of vertex attributes: %d\n", OpenGL::GetIneger(GL_MAX_VERTEX_ATTRIBS));
    glDebugMessageCallback(MyDebugMessageCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glPointSize(3.0f);
    glEnable(GL_CULL_FACE);
}

static void Render() {
    static glm::vec4 bg_color(0.9f, 0.9f, 0.9f, 1.0f);
    static glm::vec3 fg_color(0.9f, 0.8f, 0.05f);
    static glm::vec3 axis_color(0.0f, 0.2f, 0.9f);
    // init
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));
    glClear(GL_DEPTH_BUFFER_BIT);
    auto ub_xform = UBI->find("Transformations");
    assert(ub_xform);
    if (Draw::Axes) {
        OpenGL::Program::Use(*ProgramLines);
        // prepare to draw axes
        glVertexAttrib3fv(2, glm::value_ptr(axis_color));
        for (auto&& r : ub_xform->uniforms) {
            if (r.name == std::string("World_Model")) {
                glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(MyAxis.GetTransform()));
            }
        }
        // draw axes
        MyAxis.GetMesh().Draw(VAO, GL_LINES);
    }
    if (Draw::Back != Draw::None || Draw::Front != Draw::None) {
        OpenGL::Program::Use(*ProgramTriangles);
        // prepare to draw main object
        glVertexAttrib3fv(2, glm::value_ptr(fg_color));
        for (auto&& r : ub_xform->uniforms) {
            if (r.name == std::string("NDC_World")) {
                glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(NDC_View * View_World));
            } else if (r.name == std::string("World_Model")) {
                glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(MyModel.GetTransform()));
            }
        }
        // draw front face
        if (Draw::Front != Draw::None) {
            glPolygonMode(GL_FRONT_AND_BACK, Draw::Front);
            glFrontFace(GL_CCW);
            MyModel.GetMesh().Draw(VAO, GL_TRIANGLES);
            CHECK_OPENGL();
        }
        // draw back face
        if (Draw::Back != Draw::None) {
            glPolygonMode(GL_FRONT_AND_BACK, Draw::Back);
            glFrontFace(GL_CW);
            MyModel.GetMesh().Draw(VAO, GL_TRIANGLES);
            CHECK_OPENGL();
        }
    }
    // finish up
    glutSwapBuffers();
    if (GLint err = glGetError()) {
        exit(err);
    }
}

static void Reshape(GLint w, GLint h) {
    NDC_View = glm::perspective(Pi / 2, static_cast<float>(w) / h, 0.01f, 100.0f);
    glViewport(0, 0, w, h);
}

static void Every15ms(int current_ms) {
    // callback loop
    static int last_ms = current_ms;
    int delta_ms = current_ms - last_ms;
    glutTimerFunc(15, Every15ms, current_ms + 15);
    last_ms = current_ms;
    // update based on inputs
    if (Input::KeyPressed[static_cast<unsigned char>('w')]) {
        EyePos += LookDir * 0.01f;
    }
    if (Input::KeyPressed[static_cast<unsigned char>('s')]) {
        EyePos -= LookDir * 0.01f;
    }
    if (Input::KeyPressed[static_cast<unsigned char>('a')]) {
        EyePos += glm::normalize(glm::cross(Up, LookDir)) * 0.01f;
    }
    if (Input::KeyPressed[static_cast<unsigned char>('d')]) {
        EyePos -= glm::normalize(glm::cross(Up, LookDir)) * 0.01f;
    }
    // MyModel.SetPos(glm::vec3(glm::sin(current_ms * 0.001f) - 0.5f, 0.0f, glm::cos(current_ms * 0.001f) - 0.5f));
    MyModel.SetOrientation(
            glm::quat_cast(glm::rotate(glm::mat4(1.0f), delta_ms * 0.0005f, glm::vec3(0.0f, 0.0f, 1.0f))) *
            MyModel.GetOrientation());
    View_World = glm::lookAt(EyePos, EyePos + LookDir, Up);
}

static void OnClose() {
    DEBUG("Window closed");
    glutIdleFunc(nullptr);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    // glut window setup
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitContextVersion(4, 5);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Sphere");
    // glut setup window specific callbacks
    glutDisplayFunc(Render);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Input::OnKeyboard);
    glutKeyboardUpFunc(Input::OnKeyboardUp);
    glutSpecialFunc(Input::OnSpecial);
    glutMotionFunc(Input::OnMotion);
    glutMouseFunc(Input::OnMouse);
    glutCloseFunc(OnClose);
    // init glew for OpenGL 4+
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        DEBUG("Failed to init glew:%s", glewGetErrorString(err));
    } else if (GLEW_VERSION_4_5) {
        DEBUG("OpenGL 4.5 support");
    }
    // init for draw
    InitShaderProgram();
    InitDraw();
    InitMisc();
    // glut setup global callbacks
    glutIdleFunc(Render);
    glutTimerFunc(15, Every15ms, 0);
    // main loop
    glutMainLoop();
}
