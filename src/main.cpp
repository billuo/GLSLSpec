#include "Debug.hpp"
#include "Math.hpp"
#include "Mesh.hpp"
#include "OpenGL/Program.hpp"
#include "OpenGL/Shader.hpp"
#include <fstream>
#include <vector>

static GLuint VAO;

static OpenGL::Program Program;
static const OpenGL::Program::UniformBlock* pUniformBlock;
static GLuint UBO;

static Mesh MyMesh;
static glm::mat4 World_Model = glm::identity<glm::mat4>();

static glm::mat4 NDC_View = glm::identity<glm::mat4>();
static glm::mat4 View_World = glm::identity<glm::mat4>();
static glm::vec3 EyePos = glm::vec3(0.0f, 0.0f, -2.0f);
static glm::vec3 LookDir = glm::vec3(0.0f, 0.0f, 1.0f);
static const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

static bool KeyPressed[128];

void MyDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum serverity, GLsizei length,
                            const GLchar* message, const void* user);

static void InitShaderProgram() {
    // Prepare shader sources
    const std::string shader_dir("./shaders/");
    struct _pair {
        const char* const file_name;
        OpenGL::Shader* shader;
        GLenum type;
    } shaders[3] = {
        { "shader.vert", new OpenGL::Shader(), GL_VERTEX_SHADER },
        { "shader.geom", new OpenGL::Shader(), GL_GEOMETRY_SHADER },
        { "shader.frag", new OpenGL::Shader(), GL_FRAGMENT_SHADER },
    };
    // Create, compile and link shaders
    Program.Create();
    for (size_t i = 0; i < countof(shaders); ++i) {
        _pair& pair = shaders[i];
        pair.shader->Create(pair.type);
        std::ifstream file_stream;
        file_stream.open((shader_dir + pair.file_name).c_str());
        if (!file_stream) {
            DEBUG("Failed to open %s\n", (shader_dir + pair.file_name).c_str());
        }
        std::string source_string((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
        pair.shader->Source(source_string.c_str());
        pair.shader->Compile();
        Program.Attach(*pair.shader);
        pair.shader->Delete();
        delete pair.shader;
    }
    Program.Link(), Program.Use();
    // Program.List(GL_PROGRAM_INPUT);
    pUniformBlock = Program.GetUniformBlock("Transformations");
    const OpenGL::Program::UniformBlock& ub = *pUniformBlock;
    // printf("Uniforms in uniform block %s<index=%u,size=%d>:\n", ub.name.c_str(), ub.index, ub.size);
    // for (size_t i = 0; i < ub.uniforms.size(); ++i) {
    //     const OpenGL::Program::Resource& r = ub.uniforms[i];
    //     printf("#%2lu:index=%3d offset=%2d %s %s\n", i, r.index, r.offset, OpenGL::TypeString(r.type),
    //     r.name.c_str());
    // }
    // setup UBO
    glCreateBuffers(1, &UBO);
    glNamedBufferStorage(UBO, ub.size, NULL, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, ub.index, UBO);
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
    // upload vertices
    MyMesh.InitData(vertices.size());
    void* ptr = MyMesh.MapBufferVertex();
    glm::vec3* out = static_cast<glm::vec3*>(ptr);
    std::copy(vertices.begin(), vertices.end(), out);
    MyMesh.UnmapBufferVertex();
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
    // init
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));
    glClear(GL_DEPTH_BUFFER_BIT);
    // update uniforms
    for (size_t i = 0; i < pUniformBlock->uniforms.size(); ++i) {
        const OpenGL::Program::Resource& r = pUniformBlock->uniforms[i];
        /// @TODO inefficient
        if (r.name == "NDC_World") {
            glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(NDC_View * View_World));
        } else if (r.name == "World_Model") {
            glNamedBufferSubData(UBO, r.offset, OpenGL::TypeSize(r.type), glm::value_ptr(World_Model));
        }
    }
    // update attributes
    glVertexAttrib3fv(1, glm::value_ptr(glm::zero<glm::vec3>()));
    glVertexAttrib3fv(2, glm::value_ptr(fg_color));
    // draw front face
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    MyMesh.Draw(VAO, GL_TRIANGLES);
    // draw back face
    glFrontFace(GL_CW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    MyMesh.Draw(VAO, GL_TRIANGLES);
    // update front frame buffer
    glutSwapBuffers();
    if (GLint err = glGetError()) {
        exit(err);
    }
}

static void Reshape(GLint w, GLint h) {
    NDC_View = glm::perspective(Pi / 2, static_cast<float>(w) / static_cast<float>(h), 0.01f, 10.0f);
    glViewport(0, 0, w, h);
}

static void Every15ms(int current_ms) {
    // callback loop
    static int last_ms = current_ms;
    int delta_ms = current_ms - last_ms;
    glutTimerFunc(15, Every15ms, current_ms + 15);
    last_ms = current_ms;
    // update based on inputs
    if (KeyPressed[static_cast<unsigned char>('w')]) {
        EyePos += LookDir * 0.01f;
    }
    if (KeyPressed[static_cast<unsigned char>('s')]) {
        EyePos -= LookDir * 0.01f;
    }
    if (KeyPressed[static_cast<unsigned char>('a')]) {
        EyePos += glm::normalize(glm::cross(Up, LookDir)) * 0.01f;
    }
    if (KeyPressed[static_cast<unsigned char>('d')]) {
        EyePos -= glm::normalize(glm::cross(Up, LookDir)) * 0.01f;
    }

    World_Model = glm::rotate(glm::identity<glm::mat4>(), delta_ms * 0.001f, glm::vec3(1.0f, 1.0f, 0.0f)) * World_Model;
    View_World = glm::lookAt(EyePos, EyePos + LookDir, Up);
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
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    const float x_degrees(180.0f / w * -x);
    const float y_degrees(180.0f / h * -y);
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
    LookDir.x = glm::sin(RadianOfDegree(ViewAngle.horizontal)) * glm::cos(RadianOfDegree(ViewAngle.vertical));
    LookDir.y = glm::sin(RadianOfDegree(ViewAngle.vertical));
    LookDir.z = glm::cos(RadianOfDegree(ViewAngle.horizontal)) * glm::cos(RadianOfDegree(ViewAngle.vertical));
    View_World = glm::lookAt(EyePos, EyePos + LookDir, Up);
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

static void OnClose() {
    printf("Window closed\n");
    glutIdleFunc(NULL);
}

// void Idle() {}

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
    glutKeyboardFunc(OnKeyboard);
    glutKeyboardUpFunc(OnKeyboardUp);
    glutSpecialFunc(OnSpecial);
    glutMotionFunc(OnMotion);
    glutMouseFunc(OnMouse);
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
