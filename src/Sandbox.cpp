
#include <Sandbox.hpp>

#include "Log.hpp"
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
    GLbitfield stage = 0;
    switch (*type) {
        case GL_VERTEX_SHADER:
            stage = GL_VERTEX_SHADER_BIT;
            break;
        case GL_TESS_CONTROL_SHADER:
            stage = GL_TESS_CONTROL_SHADER_BIT;
            break;
        case GL_TESS_EVALUATION_SHADER:
            stage = GL_TESS_EVALUATION_SHADER_BIT;
            break;
        case GL_GEOMETRY_SHADER:
            stage = GL_GEOMETRY_SHADER_BIT;
            break;
        case GL_FRAGMENT_SHADER:
            stage = GL_FRAGMENT_SHADER_BIT;
            break;
        case GL_COMPUTE_SHADER:
            stage = GL_COMPUTE_SHADER_BIT;
            break;
        default:
            break;
    }
    auto&& source = file.fetch();
    if (!source) {
        Log::e("Failed to fetch shader source: {}", source.error());
        return;
    }
    Log::d("Importing shader {}", file.path());
    OpenGL::Shader shader(*type, "import");
    OpenGL::Program program("import");
    shader.source(source->c_str());
    shader.compile();
    program.set(GL_PROGRAM_SEPARABLE, GL_TRUE);
    program.attach(shader);
    program.link();
    m_pipeline.use_stage(program, stage);
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
    OpenGL::VertexArray::Bind(m_vao);
    valid = m_pipeline.validate();
    if (valid) {
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
