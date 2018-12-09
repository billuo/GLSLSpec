#include "Utility/Log.hpp"
#include "Sandbox.hpp"
#include "OpenGL/Constants.hpp"


std::unique_ptr<Sandbox> sandbox;

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
    GLbitfield stage = OpenGL::shaderBitOfShaderType(*type);
    auto&& source = file.fetch();
    if (!source) {
        Log::e("Failed to fetch shader source: {}", source.error());
        return;
    }
    DEBUG("Importing shader {}", file.path());
    OpenGL::Shader shader(*type, "import");
    OpenGL::Program program("import");
    shader.source(source->c_str());
    shader.compile();
    program.set(GL_PROGRAM_SEPARABLE, GL_TRUE);
    program.attach(shader);
    program.link();
    m_pipeline.use_stage(program, stage);
    auto n = OpenGL::orderOfShaderBit(stage);
    m_introspectors[n] = std::make_unique<OpenGL::Introspector>(program);
}

void
Sandbox::aux_import_image(const DynamicFile& path, const std::string& tag)
{
    // TODO
}

void
Sandbox::aux_import_geometry(const DynamicFile& path, const std::string& tag)
{
    // TODO
}

void
Sandbox::aux_import_dependency(const DynamicFile& path, const std::string& tag)
{
    // TODO
}

void
Sandbox::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (m_pipeline.valid()) {
        auto&& mvp = camera.projection_world();
        auto&& v = glm::vec3(1.0f, 1.0f, 1.0f);
        v.x = 0.0f;
        v.z = 0.0f;
        auto order = OpenGL::VertexShader;
        auto u_mvp = m_introspectors[order]->IUniform->find("MVP");
        if (u_mvp) {
            glProgramUniformMatrix4fv(m_pipeline.stage(order), u_mvp->location, 1, GL_FALSE, glm::value_ptr(mvp));
        }
        OpenGL::VertexArray::Bind(m_vao);
        glDrawArrays(GL_LINES, 0, 6);
    } else {
        Log::e("Program pipeline invalid: {}", m_pipeline.get_info_log().get());
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
