/**
 * @file Render.cpp
 * @brief Takes care of rendering
 * @author Zhen Luo 461652354@qq.com
 */
#include "Render.hpp"
#include "Debug.hpp"
#include "Math.hpp"
#include "Model.hpp"
#include "OpenGL/Debug.hpp"
#include "OpenGL/Introspection.hpp"
#include "OpenGL/Utility/ShaderCompiler.hpp"
#include "Log.hpp"

#include <memory>
#include <algorithm>


namespace Render {

namespace {
GLuint VAO, UBO;
std::shared_ptr<Model> MyModel;
std::shared_ptr<Model> Cube;

enum RasterizationMode {
    None = 0, Fill = GL_FILL, Line = GL_LINE, Point = GL_POINT
};
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

static inline RasterizationMode
NextMode(RasterizationMode mode)
{
    switch (mode) {
        case None:
            return Fill;
        case Fill:
            return Line;
        case Line:
            return Point;
        case Point:
            return None;
        default:
            return None;
    }
}

void
toggleAxes()
{ Axes = !Axes; }

void
switchRasterizationMode(Side side)
{
    if (side == Side::Back) {
        BackMode = NextMode(BackMode);
    } else if (side == Side::Front) {
        FrontMode = NextMode(FrontMode);
    }
}

void
setProjectionMatrix(const glm::mat4& mat)
{ NDC_View = mat; }

void
setViewMatrix(const glm::mat4& mat)
{ View_World = mat; }

float Shininess = 2.0f;

//
// Init
//

static void
initMisc()
{
    glDebugMessageCallback(OpenGL::OpenGLOnDebug, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE,
                          GL_DONT_CARE,
                          GL_DONT_CARE,
                          0,
                          nullptr,
                          GL_TRUE
                         );
    glPointSize(3.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

static void
initShaderProgram()
{
    using namespace OpenGL;
    const std::string dir("./shaders/");
    auto from_source = [dir](const char* file_name)
    {
        static auto c = ShaderCompiler::Instance();
        return c.compile(ShaderSource(dir + file_name));
    };
    // prepare main program
    ProgramSphere = std::make_unique<OpenGL::Program>("ADS shading");
    ProgramSphere->attach({from_source("shader.vert"),
                           from_source("shader.geom"),
                           from_source("shader.frag")}
                         ).link();
    // prepare program to draw axes
    ProgramAxes = std::make_unique<OpenGL::Program>("Axes");
    ProgramAxes->attach({from_source("axes.vert"),
                         from_source("axes.frag")}
                       ).link();
    // setup UBO
    UI_axes = std::make_unique<UniformInterface>(*ProgramAxes);
    UI = std::make_unique<UniformInterface>(*ProgramSphere);
    UBI = std::make_unique<UniformBlockInterface>(*ProgramSphere);
    PII = std::make_unique<ProgramInputInterface>(*ProgramSphere);
    POI = std::make_unique<ProgramOutputInterface>(*ProgramSphere);
#if defined(DEBUG_BUILD)
    UI_axes->dump();
    UI->dump();
    UBI->dump();
    PII->dump();
    POI->dump();
#endif
    auto ub_xform = UBI->find("Transformations");
    assert(ub_xform);
    glCreateBuffers(1, &UBO);
    glNamedBufferStorage(UBO, ub_xform->size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER,
                     static_cast<GLuint>(ub_xform->binding),
                     UBO
                    );
}

static void
sphereVetices(float radius, size_t n_slices, size_t n_layers,
              std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals)
{
    using namespace glm;
    const float dtheta = 2 * Pi / n_slices;
    const float dphi = Pi / n_layers;
    float r1 = 0.0f;
    float r2 = sin(dphi);
    float y1 = 1.0f;
    float y2 = cos(dphi);
    for (size_t layer = 0; layer <= n_layers; ++layer) {
        float theta1 = 0.0f;
        float theta2 = theta1 + dtheta;
        for (size_t slice = 0; slice < n_slices; ++slice) {
            vertices.emplace_back(cos(theta1) * r2, y2, sin(theta1) * r2);
            vertices.emplace_back(cos(theta1) * r1, y1, sin(theta1) * r1);
            vertices.emplace_back(cos(theta2) * r2, y2, sin(theta2) * r2);
            vertices.emplace_back(cos(theta2) * r2, y2, sin(theta2) * r2);
            vertices.emplace_back(cos(theta1) * r1, y1, sin(theta1) * r1);
            vertices.emplace_back(cos(theta2) * r1, y1, sin(theta2) * r1);
            theta1 = theta2;
            theta2 = theta1 + dtheta;
        }
        r1 = r2;
        r2 = sin((layer + 1) * dphi);
        y1 = y2;
        y2 = cos((layer + 1) * dphi);
    }
    normals = vertices;
    const size_t n_vertices = vertices.size();
    for (size_t i = 0; i < n_vertices; ++i) {
        vertices[i] *= radius;
    }
}

static void
cubeVertices(float a, size_t n_grids, std::vector<glm::vec3>& vertices,
             std::vector<glm::vec3>& normals)
{
    vertices.clear();
    normals.clear();
    const float delta = a / n_grids;
    float y = -a / 2;
    for (size_t i = 0; i < n_grids; ++i) {
        float x = -a / 2;
        for (size_t j = 0; j < n_grids; ++j) {
            vertices.emplace_back(x, y, a / 2);
            vertices.emplace_back(x + delta, y, a / 2);
            vertices.emplace_back(x + delta, y + delta, a / 2);
            vertices.emplace_back(x, y, a / 2);
            vertices.emplace_back(x + delta, y + delta, a / 2);
            vertices.emplace_back(x, y + delta, a / 2);
            x += delta;
        }
        y += delta;
    }
    size_t face_size = vertices.size();
    vertices.reserve(6 * face_size);
    std::transform(vertices.rbegin(),
                   vertices.rend(),
                   std::back_inserter(vertices),
                   [](const glm::vec3& v)
                   {
                       return glm::vec3(v.x, v.y, -v.z);
                   }
                  ); // NOTE the reversed_iterator
    std::transform(vertices.begin(),
                   vertices.begin() + 2 * face_size,
                   std::back_inserter(vertices),
                   [](const glm::vec3& v)
                   { return glm::vec3(-v.z, v.y, v.x); }
                  );
    std::transform(vertices.begin(),
                   vertices.begin() + 2 * face_size,
                   std::back_inserter(vertices),
                   [](const glm::vec3& v)
                   { return glm::vec3(v.x, -v.z, v.y); }
                  );
    normals.reserve(6 * face_size);
    std::fill_n(std::back_inserter(normals),
                face_size,
                glm::vec3(0.0f, 0.0f, 1.0f));
    std::fill_n(std::back_inserter(normals),
                face_size,
                glm::vec3(0.0f, 0.0f, -1.0f));
    std::fill_n(std::back_inserter(normals),
                face_size,
                glm::vec3(-1.0f, 0.0f, 0.0f));
    std::fill_n(std::back_inserter(normals),
                face_size,
                glm::vec3(1.0f, 0.0f, 0.0f));
    std::fill_n(std::back_inserter(normals),
                face_size,
                glm::vec3(0.0f, -1.0f, 0.0f));
    std::fill_n(std::back_inserter(normals),
                face_size,
                glm::vec3(0.0f, 1.0f, 0.0f));
}

static void
initMesh(Mesh& mesh, const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec3>& normals)
{
    assert(vertices.size() == normals.size());
    mesh.initData(static_cast<GLsizei>(vertices.size()));
    {
        void* ptr = mesh.mapBufferVertex();
        auto* out = static_cast<glm::vec3*>(ptr);
        std::copy(vertices.begin(), vertices.end(), out);
        mesh.unmapBufferVertex();
    }
    {
        void* ptr = mesh.mapBufferNormal();
        auto* out = static_cast<glm::vec3*>(ptr);
        std::copy(normals.begin(), normals.end(), out);
        mesh.unmapBufferNormal();
    }
}

void
init()
{
    initMisc();
    glCreateVertexArrays(1, &VAO);
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    MyModel = std::make_shared<Model>();
    Cube = std::make_shared<Model>();
    sphereVetices(0.8f, 120, 60, vertices, normals);
    initMesh(MyModel->mesh, vertices, normals);
    cubeVertices(1.0f, 50, vertices, normals);
    initMesh(Cube->mesh, vertices, normals);
    initShaderProgram();
}

//
// Render
//

static void
renderFront(Model& model)
{
    if (FrontMode != None) {
        glFrontFace(GL_CCW);
        glPolygonMode(GL_FRONT_AND_BACK, FrontMode);
        model.mesh.draw(VAO, GL_TRIANGLES);
    }
}

static void
renderBack(Model& model)
{
    if (BackMode != None) {
        glPolygonMode(GL_FRONT_AND_BACK, BackMode);
        glFrontFace(GL_CW);
        model.mesh.draw(VAO, GL_TRIANGLES);
    }
}

static void
renderFrontBack(Model& model)
{
    auto ub_xform = UBI->find("Transformations");
    auto u_model = ub_xform->find("View_Model");
    auto u_normal = ub_xform->find("NormalMatrix");
    auto u_view = ub_xform->find("NDC_View");
    auto u_mvp = ub_xform->find("NDC_Model");
    glm::mat4 View_Model = View_World * static_cast<glm::mat4>(model.transform);
    glm::mat3 NormalMatrix =
            glm::transpose(glm::inverse(glm::mat3(View_Model))); // inverse transpose matrix
    glNamedBufferSubData(UBO,
                         u_model->offset,
                         OpenGL::sizeOfDataType(static_cast<GLenum>(u_model->type)),
                         glm::value_ptr(View_Model));
    for (glm::length_t i = 0; i < glm::mat3::length(); ++i) {
        glNamedBufferSubData(UBO,
                             u_normal->offset + u_normal->mstride * i,
                             3 * sizeof(GLfloat),
                             glm::value_ptr(NormalMatrix[i]));
    }
    glNamedBufferSubData(UBO,
                         u_view->offset,
                         OpenGL::sizeOfDataType(static_cast<GLenum>(u_view->type)),
                         glm::value_ptr(NDC_View));
    glNamedBufferSubData(UBO,
                         u_mvp->offset,
                         OpenGL::sizeOfDataType(static_cast<GLenum>(u_mvp->type)),
                         glm::value_ptr(NDC_View * View_Model));
    renderFront(model);
    renderBack(model);
}

void
render()
{
    static glm::vec4 bg_color(0.9f, 0.9f, 0.9f, 1.0f);
    //
    // init
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(bg_color));
    glClear(GL_DEPTH_BUFFER_BIT);
    //
    // draw axes
    auto u_world_axes = UI_axes->find("NDC_World");
    assert(u_world_axes);
    if (Render::Axes) {
        OpenGL::Program::Use(*ProgramAxes);
        glUniformMatrix4fv(u_world_axes->location,
                           1,
                           GL_FALSE,
                           glm::value_ptr(NDC_View * View_World));
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 6);
    }
    //
    // draw main object
    OpenGL::Program::Use(*ProgramSphere);
    // light
    auto u_lpos = UI->find("Light.pos");
    auto u_la = UI->find("Light.la");
    auto u_ld = UI->find("Light.ld");
    auto u_ls = UI->find("Light.ls");
    static glm::vec3 light_pos_world = glm::vec3(4.0f, 10.0f, 4.0f);
    // glUniform3fv(u_lpos->location, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 1.0f)));
    glUniform3fv(u_lpos->location,
                 1,
                 glm::value_ptr(View_World * glm::vec4(light_pos_world, 1.0f)));
    glUniform3fv(u_la->location,
                 1,
                 glm::value_ptr(glm::vec3(0.15f, 0.15f, 0.05f)));
    glUniform3fv(u_ld->location,
                 1,
                 glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.03f)));
    glUniform3fv(u_ls->location,
                 1,
                 glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.03f)));
    // material
    auto u_ka = UI->find("Material.ka");
    auto u_kd = UI->find("Material.kd");
    auto u_ks = UI->find("Material.ks");
    auto u_shininess = UI->find("Material.shininess");
    glUniform3fv(u_ka->location,
                 1,
                 glm::value_ptr(glm::vec3(0.5f, 0.5f, 1.0f)));
    glUniform3fv(u_kd->location, 1, glm::value_ptr(glm::vec3(0.7f)));
    glUniform3fv(u_ks->location, 1, glm::value_ptr(glm::vec3(0.5f)));
    glUniform1f(u_shininess->location, Shininess);
    // render
    renderFrontBack(*MyModel);
    Cube->transform.position = glm::vec3(1.5f, 0.0f, 0.0f);
    renderFrontBack(*Cube);
    Cube->transform.position = glm::vec3(-1.5f, 0.0f, 0.0f);
    renderFrontBack(*Cube);
    //
    // finish up
    glutSwapBuffers();
}
} // namespace Render
