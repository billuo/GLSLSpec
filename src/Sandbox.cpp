/**
 * @File Sandbox.cpp
 * @author Zhen Luo 461652354@qq.com
 */

#include <Sandbox.hpp>
#include <Options.hpp>
#include <Window.hpp>
#include <tol/tiny_obj_loader.h>
#include <regex>


namespace {

void
AssignCommonUniforms(Weak<OpenGL::Introspector> intro)
{
    auto&& I = intro.lock();
    assert(I);
    auto&& uni = I->uniform();
    auto&& vp = main_window->viewport();
    auto&& fbsize = main_window->frame_buffer_size();
    auto&& mpos = main_window->mouse_position();
    mpos.y = vp.w - mpos.y; // XXX
    uni.assign(I->name, "u_viewport", vp.x, vp.y, vp.z, vp.w);
    uni.assign(I->name, "u_fbsize", fbsize.x, fbsize.y);
    uni.assign(I->name, "u_mpos", mpos.x, mpos.y);
    uni.assign(I->name, "u_time", static_cast<float>(glfwGetTime()));
//    ONCE_PER(PRINT_VALUE(mpos), 240);
}

} // namespace

const std::string axes_vert_source = R"SHADER(
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

const std::string axes_frag_source = R"SHADER(
#version 430 core
in vec3 color;
out vec4 color_out;
void main(void) {
    color_out = vec4(color, 1.0f);
})SHADER";

const std::string background_vert_source = R"SHADER(
#version 430 core
out gl_PerVertex {
    vec4 gl_Position;
};
void main() {
    const vec2[] vertices = vec2[](vec2(-1.0f, -1.0f), vec2(1.0f, -1.0f), vec2(1.0f, 1.0f), vec2(-1.0f, 1.0f));
    if (gl_VertexID < 3) {
        gl_Position.xy = vertices[gl_VertexID];
    } else {
        gl_Position.xy = vertices[(gl_VertexID - 1) % 4];
    }
    gl_Position.zw = vec2(1.0f, 1.0f);
})SHADER";

std::unique_ptr<Sandbox> sandbox;

Sandbox::Sandbox()
{
    m_pipeline_user.bind().label("user pipeline");
    // initialize internal OpenGL objects
    m_vao_internal.bind().label("internal VAO");
    m_pipeline_internal.bind().label("internal pipeline");
    // initialize internal programs
    OpenGL::Shader axes_vert(GL_VERTEX_SHADER), axes_frag(GL_FRAGMENT_SHADER), background_vert(GL_VERTEX_SHADER);
    axes_vert.source(axes_vert_source).compile();
    axes_frag.source(axes_frag_source).compile();
    background_vert.source(background_vert_source).compile();
    m_debug_axes.set(GL_PROGRAM_SEPARABLE, GL_TRUE).attach(axes_vert).attach(axes_frag).link();
    if (m_debug_axes.name() == 0) {
        ERROR("Debug drawing axes shader program invalid!");
    }
    m_background_vert.set(GL_PROGRAM_SEPARABLE, GL_TRUE).attach(background_vert).link();
    if (m_background_vert.name() == 0) {
        ERROR("Background drawing vertex shader stage invalid!");
    }
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
    auto&& ex_type = OpenGL::shaderTypeOfSuffix(tag);
    if (!ex_type) {
        Log::e("Can't determine shader type: {}", ex_type.error());
        return;
    }
    auto&& ex_source = file.fetch();
    if (!ex_source) {
        Log::e("Failed to fetch shader source: {}", ex_source.error());
        return;
    }
    GLenum type = ex_type.value();
    auto&& stage = OpenGL::stageOfShaderType(type);
    m_sources_user[underlying_cast(stage)] = std::move(*ex_source);
    m_programs_user[underlying_cast(stage)] = aux_recompile(type);
}

std::string
Sandbox::aux_preprocess_shader_source(std::string source, const std::vector<std::string>& extra_defines)
{
    // TODO anchor ('$') seems not to work??
    std::regex version("^[:space:]*#version[^\r\n]*", std::regex_constants::basic);
    std::smatch match;
    std::string::const_iterator it = source.begin();
    if (!std::regex_search(source, match, version)) {
        Log::w("Could not locate '#version' directive; defining macros in the beginning of shader...");
    } else {
        it = match[0].second + 1; // still <= source.end()
    }
    static auto&& add_define = [](std::string& str, const auto& defines)
    {
        for (auto& macro : defines) {
            std::string define = "#define " + macro + '\n';
            auto pos = define.find_first_of('=');
            if (pos != std::string::npos) {
                define[pos] = ' ';
            }
            str += define;
        }
    };
    std::string defines;
    add_define(defines, options.defines);
    add_define(defines, extra_defines);
    source.insert(it, defines.begin(), defines.end());
    return source;
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
        for (auto& shape : shapes) {
            // Log::d("group name '{}'", shape.name);
            auto& mesh = shape.mesh;
            size_t n_vertices = mesh.indices.size();
            // XXX triangulated when loading, every face in mesh should have 3 vertices
            assert(n_vertices == mesh.num_face_vertices.size() * 3);
            constexpr auto max_vertices = std::numeric_limits<GLushort>::max();
            if (n_vertices > max_vertices) {
                Log::w("Group need {} indices > soft limit={}", n_vertices, max_vertices);
                // TODO hard cap it?
            } else if (n_vertices == 0) {
                Log::w("Empty group. skipped");
                continue;
            }
            Owned<VertexBuffer<glm::vec3>> positions;
            Owned<VertexBuffer<glm::vec3>> normals;
            Owned<VertexBuffer<glm::vec2>> tex_coords;
            auto& sample = mesh.indices[0];
            using Usage = VertexAttribute::Usage;
            if (sample.vertex_index != -1) {
                positions = std::make_unique<VertexBuffer<glm::vec3>>(Usage::Position);
            }
            if (sample.normal_index != -1) {
                normals = std::make_unique<VertexBuffer<glm::vec3>>(Usage::Normal);
            }
            if (sample.texcoord_index != -1) {
                tex_coords = std::make_unique<VertexBuffer<glm::vec2>>(Usage::TexCoord);
            }
            for (auto& index : mesh.indices) {
                if (positions) {
                    assert(index.vertex_index != -1);
                    positions->add({attributes.vertices[3 * index.vertex_index],
                                    attributes.vertices[3 * index.vertex_index + 1],
                                    attributes.vertices[3 * index.vertex_index + 2]});
                }
                if (normals) {
                    assert(index.normal_index != -1);
                    normals->add({attributes.normals[3 * index.normal_index],
                                  attributes.normals[3 * index.normal_index + 1],
                                  attributes.normals[3 * index.normal_index + 2]});
                }
                if (tex_coords) {
                    assert(index.texcoord_index != -1);
                    tex_coords->add({attributes.texcoords[2 * index.texcoord_index],
                                     attributes.texcoords[2 * index.texcoord_index + 1]});
                }
            }
            m_meshes.erase(file);
            Shared<MeshBase> new_mesh(new Mesh<glm::vec3, glm::vec3, glm::vec2>(std::move(positions),
                                                                                std::move(normals),
                                                                                std::move(tex_coords)));
            auto&&[it, _] = m_meshes.emplace(file, std::move(new_mesh));
            assert(_);
            it->second->upload_all();
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
Sandbox::recompile()
{
    using Stage = OpenGL::ShaderStage;
    for (auto&&[stage, type] : {std::pair{Stage::Vertex, GL_VERTEX_SHADER},
                                {Stage::TessellationControl, GL_TESS_CONTROL_SHADER},
                                {Stage::TessellationEvaluation, GL_TESS_EVALUATION_SHADER},
                                {Stage::Geometry, GL_GEOMETRY_SHADER},
                                {Stage::Fragment, GL_FRAGMENT_SHADER},
                                {Stage::Compute, GL_COMPUTE_SHADER}}) {
        if (!m_sources_user[underlying_cast(stage)].empty()) {
            m_programs_user[underlying_cast(stage)] = aux_recompile(type);
        }
    }
    m_background_frag = aux_recompile(GL_FRAGMENT_SHADER);
    // TODO if we assume empty cache means no compling, clear cache when appropriate!
}

OpenGL::Program
Sandbox::aux_recompile(GLenum shader_type)
{
    // TODO pretty messy flow
    auto stage = OpenGL::stageOfShaderType(shader_type);
    auto&& name = OpenGL::nameOfShaderType(shader_type);
    std::string source;
    // background shader depends on user fragment shader
    if (shader_type == GL_FRAGMENT_SHADER) {
        source = aux_preprocess_shader_source(m_sources_user[underlying_cast(stage)], {"BACKGROUND"});
        auto&& program = OpenGL::Program(shader_type, {source});
        if (program.name() == 0) {
            Log::e("Background fragment shader failed to compile: {}", program.get_info_log());
        } else {
            program.label("(internal)bg-fragment");
            m_background_frag = std::move(program);
        }
    }
    // user shader
    source = aux_preprocess_shader_source(m_sources_user[underlying_cast(stage)]);
    OpenGL::Program program(shader_type, {source});
    if (program.name() == 0) {
        Log::e("Shader failed to compile: {}", program.get_info_log());
    } else {
        program.label("(imported)" + name);
        Log::i("Updated {} stage of user pipeline", name);
    }
    return program;
}

void
Sandbox::render()
{
    using Stage = OpenGL::ShaderStage;
    for (auto stage : {Stage::Vertex, Stage::TessellationControl, Stage::TessellationEvaluation, Stage::Geometry,
                       Stage::Fragment, Stage::Compute}) {
        auto& program = m_programs_user[underlying_cast(stage)];
        if (program) {
            m_pipeline_user.use_stage(program.value(), OpenGL::bitOfShaderStage(stage));
        }
    }
    // TODO can pipeline with only one stage bound to 'valid'?
    if (m_pipeline_user.valid()) {
        m_pipeline_user.bind();
        const auto& vertex_shader = m_programs_user[underlying_cast(OpenGL::ShaderStage::Vertex)];
        // TODO maybe supply a default one?
        if (!vertex_shader) {
            ONCE_PER(Log::e("No vertex shader found."), 60);
            return;
        } else if (!m_programs_user[underlying_cast(OpenGL::ShaderStage::Fragment)]) {
            ONCE_PER(Log::e("No fragment shader found."), 60);
            return;
        }
        // TODO introspection: make assignment easier
        GLuint program = vertex_shader.value().name();
        auto&& uniforms = vertex_shader.value().interfaces().lock()->uniform();
        auto&& update_mat = [&uniforms](GLuint program, const auto& mat, const char* name)
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
        update_mat(program, camera.projection_world(), "PVM");
        update_mat(program, camera.projection_view(), "PV");
        update_mat(program, camera.view_world(), "VM");
        update_mat(program, camera.normal_matrix(), "NM");
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
            mesh->draw(program);
        }
    }
}

void
Sandbox::render_background()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_background_frag.name() == 0) {
        // TODO better way of avoid binding invalid program to pipeline
        return;
    }
    m_pipeline_internal.use_stage(m_background_frag, GL_FRAGMENT_SHADER_BIT);
    m_pipeline_internal.use_stage(m_background_vert, GL_VERTEX_SHADER_BIT);
    if (m_pipeline_internal.valid()) {
        AssignCommonUniforms(m_background_frag.interfaces());
        m_pipeline_internal.bind();
        m_vao_internal.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    } else {
        ONCE_PER(ERROR("Background shader program invalid."), 60);
    }
}

void
Sandbox::render_debug()
{
    m_pipeline_internal.use_stage(m_debug_axes, GL_ALL_SHADER_BITS);
    assert(m_pipeline_internal.valid());
    m_pipeline_internal.bind();
    auto&& pvm = camera.projection_world();
    glProgramUniformMatrix4fv(m_debug_axes.name(), 0, 1, GL_FALSE, glm::value_ptr(pvm));
    m_vao_internal.bind();
    glDrawArrays(GL_LINES, 0, 6);
}



