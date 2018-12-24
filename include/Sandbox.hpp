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
#include "Window.hpp"


/// Contains everything... for now
class Sandbox {
  public:
    Sandbox(Watcher& watcher);

    void on_resize(glm::ivec2 fbsize)
    {
        aux_allocate_framebuffer_texture(fbsize);
        camera.set_aspect(static_cast<float>(fbsize.x) / fbsize.y);
    }

    void import(ImportedFile file, bool add_to_watch = false);

    /// Recompile all shaders using cached sources, when it's not the source that's updated.
    void recompile_all();

    void toggle_background();
    void render_background();

    void render();

    void toggle_postprocess();
    void render_postprocess();

    void render_debug();

    // TODO encapsulate?
    Watcher& watcher;

    // TODO encapsulate?
    Scene::Camera camera{glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0.0f)};

    /// @brief Obtain a list of program names and labels.
    /// @return The aforementioned list.
    std::vector<std::pair<GLuint, std::string>> programs();

  private:
    using Empty = OpenGL::Empty;

    /// for internal use only;
    /// based on this aux_compile() insert additional '#define's and give difference label to shader.
    enum class ShaderUsage {
        User,
        Background,
        Postprocess,
    };
    /// All user-specifiable program should be paired with a path leading to the corresponding shader source.
    struct ImportedProgram {
        ImportedFile file{}; // From which source of the program is read and compiled
        OpenGL::Program program{Empty()}; // compiled separable program
    };
    /// Compile an ImportedFile to be a single stage program and return it paired with the file.
    static ImportedProgram aux_compile(const ImportedFile& file, OpenGL::ShaderStage stage, ShaderUsage usage);
    /// Insert custom '#define's directly below '#version' directive
    /// and expand '#include's into the contents of corresponding files.
    /// @return Expected preprocessed source string. Unexpected message string if any error occurred.
    static std::string
    aux_preprocess_shader_source(std::string source, const std::vector<std::string>& extra_defines = {});

    /// Assign a bunch of uniforms, useful for every shader.
    static const OpenGL::ProgramInterface<OpenGL::Uniform>&
    aux_assign_uniforms(const OpenGL::Program& program, const Scene::Camera& camera);

    //region Forward rendering

    /// Program pipeline using stages of shader programs that were compiled from user specified shader sources.
    /// @note Should always be validated before use. Only used in render().
    OpenGL::ProgramPipeline m_pipeline_user;

    std::array<ImportedProgram, OpenGL::MaxShaderStage> m_programs_user{};

    //endregion

    //region Debug rendering

    /// VAO for internal and static usage, like when drawing background or debug curves/planes.
    /// It specifies no vertex attributes at all and is the minimum VAO OpenGL can draw with.
    OpenGL::VertexArray m_vao_internal;
    /// Program pipeline for internal usage, consisting of stages from pre-defined shader mostly.
    OpenGL::ProgramPipeline m_pipeline_internal;

    OpenGL::Program m_debug_axes; /// Debug drawing -- RGB unit axes located at world origin
    // OpenGL::Program m_debug_plane; /// Debug drawing -- x-z plane in gray grid

    //endregion

    //region Background rendering

    /// Vertex shader used to draw background (compiled from internal source)
    OpenGL::Program m_background_vert;
    /// Fragment shader used to draw background (compiled from user source)
    /// @note If non-empty, background rendering is enabled.
    ImportedProgram m_background_frag;

    //endregion

    //region Postprocessing

    /// Vertex shader used for postprocessing (compiled from internal source)
    OpenGL::Program m_postprocess_vert;
    /// Fragment shader used for postprocessing (compiled from user source)
    /// @note If non-empty, postprocessing is enabled.
    ImportedProgram m_postprocess_frag;

    OpenGL::Framebuffer m_scene; ///< FBO for postprocessing.
    OpenGL::Texture m_color_texture{Empty()}; ///< Input of postprocessing, attached to postprocessing FBO.
    OpenGL::Texture m_depth_texture{Empty()}; ///< Input of postprocessing, attached to postprocessing FBO.
    OpenGL::Sampler m_color_sampler; ///< Sampler for m_color_texture
    OpenGL::Sampler m_depth_sampler; ///< Sampler for m_depth_texture

    //endregion


    bool aux_import_shader(const ImportedFile& file);

    /// @brief Import a geometry to obtain mesh(es).
    /// @param file File to import from
    /// @return True if successfully imported and added to watch.
    bool aux_import_geometry(const ImportedFile& file);

    /// @brief User supplied meshes to draw.
    std::unordered_map<ImportedFile, Shared<MeshBase>> m_meshes;

    bool aux_import_image(const ImportedFile& file);
    bool aux_import_dependency(const ImportedFile& path);

    void aux_allocate_framebuffer_texture(glm::ivec2 fbsize);
};

extern std::unique_ptr<Sandbox> sandbox;

