/**
 * @file main.cpp
 * @brief Program entry point.
 * @author Zhen Luo 461652354@qq.com
 */
#include "Debug.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "OpenGL/Program.hpp"
#include "OpenGL/Shader.hpp"
#include <algorithm>
#include <fstream>
#include <vector>

namespace {

static GLuint VAO;

static OpenGL::Program ProgramTriangles, ProgramLines;
static Model MyModel, MyAxis;
static GLuint UBO;

static glm::mat4 NDC_View = glm::identity<glm::mat4>();
static glm::mat4 View_World = glm::identity<glm::mat4>();
static glm::vec3 EyePos = glm::vec3(0.0f, 0.0f, 2.0f);
static glm::vec3 LookDir = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

} // namespace

namespace Input {

static bool KeyPressed[128];

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
    /// @TODO
    switch (key) {
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
    std::vector<std::string> sphere_sources;
    sphere_sources.push_back("shader.vert");
    sphere_sources.push_back("shader.geom");
    sphere_sources.push_back("shader.frag");
    OpenGL::Program::InitWithShaders(ProgramTriangles, shader_dir, sphere_sources);
    std::vector<std::string> axes_sources;
    axes_sources.push_back("shader.vert");
    axes_sources.push_back("shader2.geom");
    axes_sources.push_back("shader.frag");
    OpenGL::Program::InitWithShaders(ProgramLines, shader_dir, axes_sources);
    // setup UBO
    const OpenGL::Program::UniformBlock* pub = ProgramTriangles.GetUniformBlock("Transformations");
    glCreateBuffers(1, &UBO);
    glNamedBufferStorage(UBO, pub->size, NULL, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, pub->index, UBO);
}

static void SphereVetices(float radius, size_t n_slices, size_t n_layers, std::vector<glm::vec3>& vertices) {
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
    const size_t n_vertices = vertices.size();
    for (size_t i = 0; i < n_vertices; ++i) {
        vertices[i] *= radius;
    }
}

static void InitDraw() {
    // VAO
    glCreateVertexArrays(1, &VAO);
    // prepare vertices
    std::vector<glm::vec3> vertices;
    SphereVetices(0.8f, 60, 30, vertices);
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
    glDebugMessageCallback(MyDebugMessageCallback, NULL);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glPointSize(3.0f);
    glEnable(GL_CULL_FACE);
}

static void Draw() {
    static glm::vec4 bg_color(0.9f, 0.9f, 0.9f, 1.0f);
    static glm::vec3 fg_color(0.9f, 0.1f, 0.9f);
    static glm::vec3 axis_color(0.0f, 0.2f, 0.9f);
    // init
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));
    glClear(GL_DEPTH_BUFFER_BIT);
    // update attributes
    glVertexAttrib3fv(1, glm::value_ptr(glm::zero<glm::vec3>()));
    CHECK_OPENGL();
    // draw axis
    glVertexAttrib3fv(2, glm::value_ptr(axis_color));
    ProgramLines.Use();
    const OpenGL::Program::UniformBlock* pub = ProgramLines.GetUniformBlock("Transformations");
    assert(pub);
    for (size_t i = 0; i < pub->uniforms.size(); ++i) {
        const OpenGL::Program::Resource& r = pub->uniforms[i];
        if (r.name == "World_Model") {
            glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(MyAxis.GetTransform()));
        }
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    MyAxis.GetMesh().Draw(VAO, GL_LINES);
    CHECK_OPENGL();
    // draw front face
    glVertexAttrib3fv(2, glm::value_ptr(fg_color));
    ProgramTriangles.Use();
    for (size_t i = 0; i < pub->uniforms.size(); ++i) {
        const OpenGL::Program::Resource& r = pub->uniforms[i];
        if (r.name == "NDC_World") {
            glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(NDC_View * View_World));
        } else if (r.name == "World_Model") {
            glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(MyModel.GetTransform()));
        }
    }
    glFrontFace(GL_CCW);
    MyModel.GetMesh().Draw(VAO, GL_TRIANGLES);
    CHECK_OPENGL();
    // draw back face
    glFrontFace(GL_CW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    MyModel.GetMesh().Draw(VAO, GL_TRIANGLES);
    CHECK_OPENGL();
    // update front frame buffer
    glutSwapBuffers();
    if (GLint err = glGetError()) {
        exit(err);
    }
}

static void Reshape(GLint w, GLint h) {
    NDC_View = glm::perspective(Pi / 2, static_cast<float>(w) / h, 0.01f, 100.0f);
    std::string str = glm::to_string(NDC_View).c_str();
    DEBUG("Perspective:\n%s\n", str.c_str());
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
    MyModel.SetOrientation(
            glm::quat_cast(glm::rotate(glm::mat4(1.0f), delta_ms * 0.001f, glm::vec3(0.0f, 1.0f, 0.0f))) *
            MyModel.GetOrientation());
    View_World = glm::lookAt(EyePos, EyePos + LookDir, Up);
}

static void OnClose() {
    DEBUG("Window closed\n");
    glutIdleFunc(NULL);
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
    glutDisplayFunc(Draw);
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
        DEBUG("Failed to init glew:%s\n", glewGetErrorString(err));
    } else if (GLEW_VERSION_4_5) {
        DEBUG("OpenGL 4.5 support\n");
    }
    // init for draw
    InitShaderProgram();
    InitDraw();
    InitMisc();
    // glut setup global callbacks
    glutIdleFunc(Draw);
    glutTimerFunc(15, Every15ms, 0);
    // main loop
    glutMainLoop();
}
