/**
 * @File Sandbox.cpp
 * @author Zhen Luo 461652354@qq.com
 */

#include <Sandbox.hpp>
#include <OpenGL/Constants.hpp>
#include <Utility/Log.hpp>
#include <OpenGL/VertexLayout.hpp>
#include <tol/tiny_obj_loader.h>


const char* const axes_vert = R"SHADER(
#version 430 core
layout(location = 0) uniform mat4 PVM;
out gl_PerVertex {
    vec4 gl_Position;
};
out vec3 color;
vec4 ndc_world(vec3 pos){
    mat4 scale = mat4(1.0f);
    scale[3][3] = 1.0f;
    return PVM * scale * vec4(pos, 1.0f);
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
})SHADER";

const char* const axes_frag = R"SHADER(
#version 430 core
in vec3 color;
out vec4 color_out;
void main(void) { color_out = vec4(color, 1.0f); }
)SHADER";

std::unique_ptr<Sandbox> sandbox;

Sandbox::Sandbox()
{
    m_pipeline.bind().label("sandbox-pipeline");
    m_pipeline_debug.bind().label("sandbox-pipeline-d");
    m_pipeline_debug.use_stage(OpenGL::Program(GL_VERTEX_SHADER, {axes_vert}), GL_VERTEX_SHADER_BIT);
    m_pipeline_debug.use_stage(OpenGL::Program(GL_FRAGMENT_SHADER, {axes_frag}), GL_FRAGMENT_SHADER_BIT);
    if (!m_pipeline_debug.valid()) {
        ERROR("Debug pipeline is invalid!");
    }
    m_vao_debug.bind().label("sandbox-VAO-d");
}

void
Sandbox::import(const DynamicFile& file)
{
    auto& tag = file.tag;
    switch (file.type()) {
        case FileType::Shader:
            this->aux_import_shader(file, tag);
            break;
        case FileType::Image:
            aux_import_image(file, tag);
            break;
        case FileType::Geometry:
            aux_import_geometry(file, tag);
            break;
        case FileType::Dependency:
            aux_import_dependency(file, tag);
            break;
    }
}

void
Sandbox::aux_import_shader(const DynamicFile& file, const std::string& tag)
{
    auto&& type = OpenGL::shaderTypeOfSuffix(tag);
    if (!type) {
        Log::e("Can't determine shader type: {}", type.error());
        return;
    }
    GLbitfield stage = OpenGL::bitOfShaderType(*type);
    auto&& source = file.fetch();
    if (!source) {
        Log::e("Failed to fetch shader source: {}", source.error());
        return;
    }
//    DEBUG("Importing shader {}", file.path());
    OpenGL::Shader shader(*type);
    OpenGL::Program program;
    shader.label("import").source(source.value()).compile();
    program.label("import").set(GL_PROGRAM_SEPARABLE, GL_TRUE).attach(shader).link();
    if (program.name()) {
        m_pipeline.use_stage(program, stage);
        auto n = underlying_cast(OpenGL::stageOfShaderBit(stage));
        m_introspectors[n] = std::make_shared<OpenGL::Introspector>(program);
    } else {
        Log::e("Shader failed to compile.");
    }
}

void
Sandbox::aux_import_image(const DynamicFile& file, const std::string& tag)
{
    // TODO
    throw unimplemented("Load image");
}

void
Sandbox::aux_import_geometry(const DynamicFile& file, const std::string& tag)
{
    using namespace OpenGL;
    std::string extension(tag);
    for (auto& c : extension) {
        c = static_cast<char>(std::toupper(c));
    }
    if (extension == "OBJ") {
//        DEBUG("Importing wave front .obj: {}", file.path());
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        bool result = tinyobj::LoadObj(&attributes, &shapes, &materials, &err, file.path().c_str(), nullptr, true);
        if (!err.empty()) {
            Log::w("When loading {}:\n{}", file.path().string(), err);
        }
        if (!result) {
            Log::e("Failed to load .obj file.");
            return;
        }
        m_meshes.clear(); // XXX
        for (auto& shape : shapes) {
            Log::d("group name '{}'", shape.name);
            auto& mesh = shape.mesh;
            size_t n_vertices = mesh.indices.size();
            // XXX triangulated when loading, every face in mesh should have 3 vertices
            assert(n_vertices == mesh.num_face_vertices.size() * 3);
            constexpr auto max_vertices = std::numeric_limits<GLushort>::max();
            if (n_vertices > max_vertices) {
                Log::w("Group need {} indices > soft limit={}", n_vertices, max_vertices);
                // cap it?
            } else if (n_vertices == 0) {
                Log::w("Empty group. skipped");
                continue;
            }
            VertexLayout layout;
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> tex_coords;
            std::vector<glm::vec3> colors;
            auto& sample = mesh.indices[0];
            using Usage = VertexAttribute::Usage;
            if (sample.vertex_index != -1) {
                layout.define(VertexAttribute(Usage::Position, 3, GL_FLOAT));
            }
            if (sample.normal_index != -1) {
                layout.define(VertexAttribute(Usage::Normal, 3, GL_FLOAT));
            }
            if (sample.texcoord_index != -1) {
                layout.define(VertexAttribute(Usage::TexCoord, 2, GL_FLOAT));
            }
            for (auto& index : mesh.indices) {
                if (layout.attribute(Usage::Position)) {
                    assert(index.vertex_index != -1);
                    positions.emplace_back(attributes.vertices[3 * index.vertex_index],
                                           attributes.vertices[3 * index.vertex_index + 1],
                                           attributes.vertices[3 * index.vertex_index + 2]);
                }
                if (layout.attribute(Usage::Normal)) {
                    assert(index.normal_index != -1);
                    normals.emplace_back(attributes.normals[3 * index.normal_index],
                                         attributes.normals[3 * index.normal_index + 1],
                                         attributes.normals[3 * index.normal_index + 2]);
                }
                if (layout.attribute(Usage::TexCoord)) {
                    assert(index.texcoord_index != -1);
                    tex_coords.emplace_back(attributes.texcoords[2 * index.texcoord_index],
                                            attributes.texcoords[2 * index.texcoord_index + 1]);
                }
            }
            m_meshes.emplace(std::make_pair(file,
                                            Mesh(std::move(layout),
                                                 std::move(positions),
                                                 std::move(normals),
                                                 std::move(tex_coords),
                                                 std::move(colors))));
        }
    } else if (extension == "PLY") {
        // TODO
        throw unimplemented("Load .ply");
    }
}

void
Sandbox::aux_import_dependency(const DynamicFile& path, const std::string& tag)
{
    // TODO
    throw unimplemented("Load dependencies");
}

void
Sandbox::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_pipeline.valid()) {
        m_pipeline.bind();
        constexpr auto stage = OpenGL::ShaderStage::Vertex;
        const auto program = m_pipeline.stage(stage);
        const auto vertex_introspect = m_introspectors[underlying_cast(stage)];
        if (!vertex_introspect) {
            ONCE_PER(Log::e("No vertex shader specified."), 60);
            return;
        }
        auto&& uniforms = vertex_introspect->uniform();
        auto&& update_mat = [&uniforms, program](const auto& mat, const char* name)
        {
            auto* u = uniforms.find(name);
            if (!u)
                return;
            switch (sizeof(mat)) {
                case sizeof(glm::mat4):
                    return glProgramUniformMatrix4fv(program, u->location, 1, GL_FALSE, glm::value_ptr(mat));
                case sizeof(glm::mat3):
                    return glProgramUniformMatrix3fv(program, u->location, 1, GL_FALSE, glm::value_ptr(mat));
            }
        };
        update_mat(camera.projection_world(), "PVM");
        update_mat(camera.projection_view(), "PV");
        update_mat(camera.view_world(), "VM");
        update_mat(camera.normal_matrix(), "NM");
        auto&& lpos = camera.world_to_view({4.0f, 10.0f, 4.0f});
        uniforms.assign(program, "L.pos", lpos.x, lpos.y, lpos.z);
        uniforms.assign(program, "L.la", 0.15f, 0.15f, 0.05f);
        uniforms.assign(program, "L.ld", 0.8f, 0.8f, 0.03f);
        uniforms.assign(program, "L.ls", 0.8f, 0.8f, 0.03f);
        uniforms.assign(program, "M.ka", 0.5f, 0.5f, 1.0f);
        uniforms.assign(program, "M.kd", 0.7f, 0.7f, 0.7f);
        uniforms.assign(program, "M.ks", 0.5f, 0.5f, 0.5f);
        uniforms.assign(program, "M.shininess", 16.0f);
        for (auto&&[file, mesh] : m_meshes) {
            mesh.draw(vertex_introspect);
        }
    }
}

void
Sandbox::render_debug() const
{
    if (m_pipeline_debug.valid()) {
        m_pipeline_debug.bind();
        m_vao_debug.bind();
        auto stage = OpenGL::ShaderStage::Vertex;
        auto&& pvm = camera.projection_world();
        glProgramUniformMatrix4fv(m_pipeline_debug.stage(stage), 0, 1, GL_FALSE, glm::value_ptr(pvm));
        glDrawArrays(GL_LINES, 0, 6);
    }
}

void
Sandbox::update()
{
    std::lock_guard guard(mutex_updated);
    for (auto&& file : m_updated) {
        import(file);
    }
    m_updated.clear();
}

void
Sandbox::on_update(const DynamicFile& file)
{
    std::lock_guard guard(mutex_updated);
    m_updated.insert(file);
}


