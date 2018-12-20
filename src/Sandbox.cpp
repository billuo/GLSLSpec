/**
 * @File Sandbox.cpp
 * @author Zhen Luo 461652354@qq.com
 */

#include <Sandbox.hpp>
#include <Options.hpp>
#include <Window.hpp>
#include <tol/tiny_obj_loader.h>
#include <regex>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"


namespace {

// TODO temp


    const glm::vec3 tex0[] = {
            glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f),
            glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f),
            glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
            glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
    };
    const glm::vec3 tex1[] = {
            glm::vec3(1.0f), glm::vec3(0.0f),
            glm::vec3(0.0f), glm::vec3(1.0f),
    };

} // namespace

// TODO
const std::string default_vert_source = R"SHADER(
#version 430 core
uniform mat4 PVM;
out gl_PerVertex {
    vec4 gl_Position;
};
in vec3 v_position;
void main() {
    gl_Position = PVM * vec4(v_position, 1.0f);
})SHADER";

const std::string axes_vert_source = R"SHADER(
#version 430 core
layout(location = 0) uniform mat4 PVM;
layout(location = 1) uniform float internal_axes_scale;
out gl_PerVertex {
    vec4 gl_Position;
};
out vec3 color;
vec4 ndc_world(vec3 pos){
    mat4 scale = mat4(internal_axes_scale);
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

Sandbox::Sandbox(Watcher &watcher) : watcher(watcher) {
    m_pipeline_user.bind().label("[user]");
    // initialize internal OpenGL objects
    m_vao_internal.bind().label("[internal]");
    m_pipeline_internal.bind().label("[internal]");
    // initialize internal programs
    OpenGL::Shader axes_vert(GL_VERTEX_SHADER), axes_frag(GL_FRAGMENT_SHADER);
    axes_vert.source(axes_vert_source).compile();
    axes_frag.source(axes_frag_source).compile();
    m_debug_axes.set(GL_PROGRAM_SEPARABLE, GL_TRUE).attach(axes_vert).attach(axes_frag).link();
    if (m_debug_axes.name() == 0) {
        ERROR("Debug drawing axes shader program invalid!");
    } else {
        m_debug_axes.label("[internal]axes");
    }
    // initialize background vertex shader
    OpenGL::Shader background_vert(GL_VERTEX_SHADER);
    background_vert.source(background_vert_source).compile();
    m_background_vert.set(GL_PROGRAM_SEPARABLE, GL_TRUE).attach(background_vert).link();
    if (m_background_vert.name() == 0) {
        ERROR("Background drawing vertex shader stage invalid!");
    } else {
        m_background_vert.label("[background]vertex");
    }
    // initialize framebuffer
#if 0 // TODO below are temp
    m_scene.bind(GL_FRAMEBUFFER).label("[scene]");
    auto&& fbsize = main_window->frame_buffer_size();
    color_texture.bind(GL_TEXTURE_2D);
    color_texture.Storage(GL_TEXTURE_2D, 1, GL_RGBA8, fbsize.x, fbsize.y);
    color_texture.activate(0);
    color_sampler.bind(0);
    color_sampler.set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    color_sampler.set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    depth_texture.bind(GL_TEXTURE_2D);
    depth_texture.Storage(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, fbsize.x, fbsize.y);
    depth_texture.activate(1);
    depth_sampler.bind(1);
    m_scene.Attach(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
    m_scene.Attach(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    m_scene.unbind(GL_FRAMEBUFFER);
#endif
}

void
Sandbox::import(ImportedFile file, bool add_to_watch) {
    if (file.path().empty()) {
        Log::w("File to import has empty path, ignored...");
        return;
    }
    Log::i("Importing {}", file.path());
    bool result;
    switch (file.type()) {
        case FileType::Shader:
            result = aux_import_shader(file);
            break;
        case FileType::Image:
            result = aux_import_image(file);
            break;
        case FileType::Geometry:
            result = aux_import_geometry(file);
            break;
        case FileType::Dependency:
            result = aux_import_dependency(file);
            break;
        default:
            Log::e("File has unknown file type: {}", file.path());
            return;
    }
    if (result && add_to_watch) {
        watcher.watch(file.path(), file.type(), file.tag);
    }
}

bool
Sandbox::aux_import_shader(const ImportedFile &file) {
    auto ex_type = OpenGL::suffix_shader_type(file.path().extension());
    if (!ex_type) {
        Log::e("Can't determine shader type: {}", ex_type.error());
        return false;
    }
    auto stage = OpenGL::shader_type_stage(*ex_type);
    auto n = underlying_cast(stage);
    if (file.tag == "user") {
        m_programs_user[n] = aux_compile(file, stage, ShaderUsage::User);
        Log::i("User shader {} imported", m_programs_user[n].program.label());
    } else if (file.tag == "background") {
        m_background_frag = aux_compile(file, stage, ShaderUsage::Background);
        Log::i("Background shader {} imported", m_background_frag.program.label());
    } else if (file.tag == "postprocess") {
        m_programs_postprocess[n] = aux_compile(file, stage, ShaderUsage::Postprocess);
        Log::i("Postprocess shader {} imported", m_programs_postprocess[n].program.label());
    }
    return true;
}

std::string
Sandbox::aux_preprocess_shader_source(std::string source, const std::vector<std::string> &extra_defines) {
    // TODO anchor ('$') seems not to work??
    std::regex version("^[:space:]*#version[^\r\n]*", std::regex_constants::basic);
    std::smatch match;
    std::string::const_iterator it = source.begin();
    if (!std::regex_search(source, match, version)) {
        Log::w("Could not locate '#version' directive; defining macros in the beginning of shader...");
    } else {
        it = match[0].second + 1; // still <= source.end()
    }
    static auto &&add_define = [](std::string &str, const auto &defines) {
        for (auto &macro : defines) {
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

bool
Sandbox::aux_import_image(const ImportedFile &file) {
    int x, y, n;
    unsigned char *data = stbi_load(file.path().c_str(), &x, &y, &n, 0);
    OpenGL::Texture tex_obj;
    if (file.tag == "cubemap") { // TODO
        tex_obj.Storage()
    } else if (file.tag == "spheremap") { // TODO

    } else if (file.tag == "texture") {
        
    } else {
        Log::w("Unimplemented tag name {}", file.tag);
    }

//    throw unimplemented("Load image");
    return true;
}

bool
Sandbox::aux_import_geometry(const ImportedFile &file) {
    using namespace OpenGL;
    std::string extension(file.path().extension());
    for (auto &c : extension) {
        c = static_cast<char>(std::toupper(c));
    }
    if (extension == ".OBJ") {
        tinyobj::attrib_t attributes;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;
        auto path = file.path();
        auto parent_path = file.path();
        parent_path.remove_filename();
        bool result = tinyobj::LoadObj(&attributes, &shapes, &materials, &err, path.c_str(), parent_path.c_str(), true);
        // TODO use materials
        if (!err.empty()) {
            Log::w("{}", err);
        }
        if (!result) {
            Log::e("Failed to load .obj file.");
            return false;
        }
        for (auto &shape : shapes) {
            DEBUG("group name '{}'", shape.name);
            auto &mesh = shape.mesh;
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
            {
                VertexBuffer<glm::vec3>::MappedPtr ppos;
                VertexBuffer<glm::vec3>::MappedPtr pnorms;
                VertexBuffer<glm::vec2>::MappedPtr puvs;
                auto &sample = mesh.indices[0];
                using Usage = VertexAttribute::Usage;
                if (sample.vertex_index != -1) {
                    positions = std::make_unique<VertexBuffer<glm::vec3>>(Usage::Position);
                    positions->data(mesh.indices.size() * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
                    ppos = positions->map();
                }
                if (sample.normal_index != -1) {
                    normals = std::make_unique<VertexBuffer<glm::vec3>>(Usage::Normal);
                    normals->data(mesh.indices.size() * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
                    pnorms = normals->map();
                }
                if (sample.texcoord_index != -1) {
                    tex_coords = std::make_unique<VertexBuffer<glm::vec2>>(Usage::TexCoord);
                    tex_coords->data(mesh.indices.size() * sizeof(glm::vec2), nullptr, GL_STATIC_DRAW);
                    puvs = tex_coords->map();
                }
                for (auto &index : mesh.indices) {
                    if (positions) {
                        *ppos = {attributes.vertices[3 * index.vertex_index],
                                 attributes.vertices[3 * index.vertex_index + 1],
                                 attributes.vertices[3 * index.vertex_index + 2]};
                        ++ppos;
                    }
                    if (normals) {
                        *pnorms = {attributes.normals[3 * index.normal_index],
                                   attributes.normals[3 * index.normal_index + 1],
                                   attributes.normals[3 * index.normal_index + 2]};
                        ++pnorms;
                    }
                    if (tex_coords) {
                        *puvs = {attributes.texcoords[2 * index.texcoord_index],
                                 attributes.texcoords[2 * index.texcoord_index + 1]};
                        ++puvs;
                    }
                }
            }
            m_meshes.erase(file);
            Shared<MeshBase> new_mesh(new Mesh<glm::vec3, glm::vec3, glm::vec2>(mesh.indices.size(),
                                                                                std::move(positions),
                                                                                std::move(normals),
                                                                                std::move(tex_coords)));
            m_meshes.emplace(file, std::move(new_mesh));
            break; // TODO for now, only load and draw the first mesh(shape)
        }
        return true;
    } else if (extension == ".PLY") {
        // TODO
        throw unimplemented("Load .ply");
        return true;
    }
    ERROR("Unknown extension of geometry: {}", extension);
    return false;
}

bool
Sandbox::aux_import_dependency(const ImportedFile &path) {
    // TODO import dependencies of shader (with in-source '#include')
    throw unimplemented("Load dependencies");
    return true;
}

void
Sandbox::recompile_all() {
    using Stage = OpenGL::ShaderStage;
    static const std::vector<Stage> stages = {Stage::Vertex, Stage::TessellationControl,
                                              Stage::TessellationEvaluation, Stage::Geometry,
                                              Stage::Fragment, Stage::Compute};
    static auto recompile_it = [](ImportedProgram &imported, auto stage, auto usage) {
        imported = aux_compile(imported.file, stage, usage);
    };
    for (auto stage : stages) {
        recompile_it(m_programs_user[underlying_cast(stage)], stage, ShaderUsage::User);
        recompile_it(m_programs_postprocess[underlying_cast(stage)], stage, ShaderUsage::Postprocess);
    }
    recompile_it(m_background_frag, Stage::Fragment, ShaderUsage::Background);
}

Sandbox::ImportedProgram
Sandbox::aux_compile(const ImportedFile &file, OpenGL::ShaderStage stage, ShaderUsage usage) {
    auto &&ex_source = file.fetch();
    if (!ex_source) {
        Log::e("Failed to read content of {}:\n\t{}", file.path(), ex_source.error());
        return {file, OpenGL::Empty()};
    }
    auto type = OpenGL::shader_stage_type(stage);
    auto &&name = OpenGL::shader_type_name(type);
    auto &source = *ex_source;
    std::string label;
    switch (usage) {
        case ShaderUsage::User:
            label = "[user]" + name;
            source = aux_preprocess_shader_source(source, {});
            break;
        case ShaderUsage::Background:
            if (stage != OpenGL::ShaderStage::Fragment) {
                ERROR("Only fragment shader can be specified in background phase rendering");
                return {file, OpenGL::Empty()};
            }
            label = "[background]" + name;
            source = aux_preprocess_shader_source(source, {"BACKGROUND"});
            break;
        case ShaderUsage::Postprocess:
            label = "[postprocess]" + name;
            source = aux_preprocess_shader_source(source, {"POSTPROCESS"});
            break;
        default:
            UNREACHABLE;
    }
    auto program = OpenGL::Program(type, {source});
    if (program.name() == 0) {
        Log::e("Shader failed to compile: {}", program.get_info_log());
        return {file, OpenGL::Empty()};
    }
    program.label(label);
    return {file, std::move(program)};
}

void
Sandbox::render() {
    // if postprocessing is enabled, render into custom framebuffer rather than the default one.
    if (m_scene.name() != 0) {
        m_scene.bind(GL_FRAMEBUFFER);
    }
    using Stage = OpenGL::ShaderStage;
    for (auto stage : {Stage::Vertex, Stage::TessellationControl, Stage::TessellationEvaluation, Stage::Geometry,
                       Stage::Fragment, Stage::Compute}) {
        auto&&[_, program] = m_programs_user[underlying_cast(stage)];
        if (program.name()) {
            m_pipeline_user.use_stage(program, OpenGL::shader_stage_bit(stage));
        }
    }
    // TODO can pipeline with only one stage bound be 'valid'?
    if (m_pipeline_user.valid()) {
        m_pipeline_user.bind();
        const auto&[_, vertex_shader] = m_programs_user[underlying_cast(Stage::Vertex)];
        // TODO maybe provide a default one?
        if (vertex_shader.name() == 0) {
            ONCE_PER(Log::e("No vertex shader found."), 60);
            return;
        } else if (m_programs_user[underlying_cast(Stage::Fragment)].program.name() == 0) {
            ONCE_PER(Log::e("No fragment shader found."), 60);
            return;
        };
        auto &&uniforms = aux_assign_uniforms(vertex_shader, camera);
        // TODO material and illumination is per-mesh at least.
        GLuint name = vertex_shader.name();
        uniforms.assign(name, "L.pos", camera.world_to_view({4.0f, 10.0f, 4.0f}));
        uniforms.assign(name, "L.la", 0.15f, 0.15f, 0.05f);
        uniforms.assign(name, "L.ld", 0.8f, 0.8f, 0.03f);
        uniforms.assign(name, "L.ls", 0.8f, 0.8f, 0.03f);
        uniforms.assign(name, "M.ka", 0.5f, 0.5f, 1.0f);
        uniforms.assign(name, "M.kd", 0.7f, 0.7f, 0.7f);
        uniforms.assign(name, "M.ks", 0.5f, 0.5f, 0.5f);
        uniforms.assign(name, "M.shininess", 16.0f);
        for (auto&&[file, mesh] : m_meshes) {
            mesh->draw(name);
        }
    }
}

void
Sandbox::render_postprocess() {
    // If postprocessing is disabled, do nothing.
    if (m_scene.name() == 0) {
        return;
    }
    // TODO all!
    using Stage = OpenGL::ShaderStage;
    for (auto stage : {Stage::Vertex, Stage::TessellationControl, Stage::TessellationEvaluation, Stage::Geometry,
                       Stage::Fragment, Stage::Compute}) {
        auto&[_, program] = m_programs_user[underlying_cast(stage)];
        if (program.name()) {
            m_pipeline_internal.use_stage(program, OpenGL::shader_stage_bit(stage));
        }
    }
    // Otherwise, now we render into the default framebuffer.
    m_scene.unbind(GL_FRAMEBUFFER);
    if (m_pipeline_internal.valid()) {
        m_pipeline_internal.bind();
        const auto&[_, vertex_shader] = m_programs_user[underlying_cast(Stage::Vertex)];
        aux_assign_uniforms(vertex_shader, camera);
        GLuint name = vertex_shader.name();
        for (auto&&[file, mesh] : m_meshes) {
            mesh->draw(name);
        }
    }
}

void
Sandbox::render_background() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_background_frag.program.name() == 0) {
        return;
    }
    m_pipeline_internal.use_stage(m_background_frag.program, GL_FRAGMENT_SHADER_BIT);
    m_pipeline_internal.use_stage(m_background_vert, GL_VERTEX_SHADER_BIT);
    if (m_pipeline_internal.valid()) {
        aux_assign_uniforms(m_background_frag.program, camera);
        m_pipeline_internal.bind();
        m_vao_internal.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    } else {
        ONCE_PER(ERROR("Background shader program invalid."), 60);
    }
}

void
Sandbox::render_debug() {
    m_pipeline_internal.use_stage(m_debug_axes, GL_ALL_SHADER_BITS);
    assert(m_pipeline_internal.valid());
    m_pipeline_internal.bind();
    auto &&pvm = camera.projection_world();
    glProgramUniformMatrix4fv(m_debug_axes.name(), 0, 1, GL_FALSE, glm::value_ptr(pvm));
    glProgramUniform1f(m_debug_axes.name(), 1, 1.0f);
    m_vao_internal.bind();
    glDrawArrays(GL_LINES, 0, 6);
}

std::vector<std::pair<GLuint, std::string>>
Sandbox::programs() {
    std::vector<std::pair<GLuint, std::string>> ret;
    for (auto&&[_, program] : m_programs_user) {
        if (program.name() != 0) {
            ret.emplace_back(program.name(), program.label());
        }
    }
    auto &background_frag = m_background_frag.program;
    if (background_frag.name() != 0) {
        ret.emplace_back(background_frag.name(), background_frag.label());
    }
    return ret;
}

const OpenGL::ProgramInterface<OpenGL::Uniform> &
Sandbox::aux_assign_uniforms(const OpenGL::Program &program, const Scene::Camera &camera) {
    auto &&uni = program.interfaces().lock()->uniform();
    GLuint name = program.name();
    auto &&vp = main_window->viewport();
    auto &&mpos = main_window->mouse_position();
    mpos.y = vp.w - mpos.y; // XXX
    uni.assign(name, "u_viewport", vp);
    uni.assign(name, "u_fbsize", main_window->frame_buffer_size());
    uni.assign(name, "u_mpos", mpos);
    uni.assign(name, "u_time", static_cast<float>(glfwGetTime()));
    uni.assign(name, "u_camera", camera.transform().position);
    uni.assign(name, "u_camera_clip", camera.clip());
    uni.assign(name, "PVM", camera.projection_world());
    uni.assign(name, "PV", camera.projection_view());
    uni.assign(name, "VM", camera.view_world());
    uni.assign(name, "NM", camera.normal_matrix());
    return uni;
}

void
Sandbox::toggle_background() {
    if (m_background_frag.program.name() == 0) {
        if (m_background_frag.file.path().empty()) {
            Log::w("Background shader not yet specified; can not enable");
            return;
        }
        m_background_frag = aux_compile(m_background_frag.file, OpenGL::ShaderStage::Fragment, ShaderUsage::Background);
    } else {
        m_background_frag.program = OpenGL::Empty();
    }
}

