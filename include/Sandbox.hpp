/**
 * @File Sandbox.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Mesh.hpp>
#include <Scene/Camera.hpp>
#include <Watcher.hpp>
#include <OpenGL/Constants.hpp>
#include <OpenGL/Object/Buffer.hpp>
#include <OpenGL/Object/ProgramPipeline.hpp>
#include <OpenGL/Object/Texture.hpp>
#include <OpenGL/Object/Sampler.hpp>
#include <OpenGL/Object/VertexArray.hpp>
#include <OpenGL/Object/Framebuffer.hpp>


/// Contains everything... for now
class Sandbox {
  public:
    Sandbox();

    void import(const DynamicFile& file);

    /// Recompile all shaders using cached sources, when it's not the source that's updated.
    void recompile();

    void render_background();

    void render();

    void render_postprocess();

    void render_debug();

    // TODO encapsulate!
    Scene::Camera camera{glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3()};

    /// @brief Obtain a list of program names and labels.
    /// @return The aforementioned list.
    std::vector<std::pair<GLuint, std::string>> programs();

  private:
    using Empty = OpenGL::Empty;
    // struct StageProgram{
    //     FS::path path; // path to the source to compile into this program
    //     OpenGL::Program program; // compiled separable program
    // };

    /// User supplied meshes to draw.
    std::unordered_map<DynamicFile, Shared<MeshBase>> m_meshes;

    /// Program pipeline using stages of shader programs that were compiled from user specified shader sources.
    /// @note Should always be validated before use. Only used in render().
    OpenGL::ProgramPipeline m_pipeline_user;
    /// Cache of shader source. Used when file isn't updated, but more macros are defined.
    /// @note Contents of these sources are consistent with their source files, with no pre-processing involved.
    std::array<std::string, OpenGL::MaxShaderStage> m_sources_user;
    /// User shader programs, theoretically all optional.
    std::array<OpenGL::Program, OpenGL::MaxShaderStage>
            m_programs_user{Empty(), Empty(), Empty(), Empty(), Empty(), Empty()};

    /// Vertex shader stage used to draw background, compiled from internal shader.
    OpenGL::Program m_background_vert;
    /// Fragment shader stage used to draw background, compiled from user fragment shader.
    OpenGL::Program m_background_frag{Empty()};

    /// When postprocessing is enabled, scene is rendered to this buffer.
    OpenGL::Framebuffer m_scene{Empty()};

    /// VAO for internal and static usage, like when drawing background or debug curves/planes.
    /// It specifies no vertex attributes at all and is the minimum VAO OpenGL can draw with.
    OpenGL::VertexArray m_vao_internal;
    /// Program pipeline for internal usage, consisting of stages from pre-defined shader mostly.
    OpenGL::ProgramPipeline m_pipeline_internal;
    /// Debug drawing -- RGB unit axes located at world origin
    OpenGL::Program m_debug_axes;

    void aux_import_shader(const DynamicFile& file, const std::string& tag);
    /// Insert custom '#define's directly below '#version' directive
    /// and expand '#include's into the contents of corresponding files.
    /// @return Expected preprocessed source string. Unexpected message string if any error occurred.
    expected<std::string, std::string>
    aux_preprocess_shader_source(std::string source, const std::vector<std::string>& extra_defines = {});
    /// Recompile a particular user shader with cached source. May compile other internal shaders that depend on user shaders as well.
    OpenGL::Program aux_recompile(GLenum shader_type);
    /// Assign a bunch of uniforms, useful for every shader.
    const OpenGL::ProgramInterface<OpenGL::Uniform>& aux_assign_uniforms(const OpenGL::Program& program);

    void aux_import_image(const DynamicFile& file, const std::string& tag);
    void aux_import_geometry(const DynamicFile& file, const std::string& tag);
    void aux_import_dependency(const DynamicFile& path, const std::string& tag);
};

extern std::unique_ptr<Sandbox> sandbox;

