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
#include <OpenGL/Object/VertexArray.hpp>


class Sandbox {
  public:
    Sandbox();

    void import(const DynamicFile& file);

    void render();

    void render_debug();

    void render_background();

    Scene::Camera camera{glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3()};

  private:

    std::set<DynamicFile> m_updated;
    std::mutex mutex_updated;

    /// Program pipeline using stages of shader programs that were compiled from user specified shader sources.
    /// @note Should always be validated before use.
    OpenGL::ProgramPipeline m_pipeline_user;
    /// User shader programs, theoretically all optional.
    std::array<std::optional<OpenGL::Program>, OpenGL::MaxShaderStage> m_programs_user;
    /// User supplied meshes to draw.
    std::unordered_map<DynamicFile, Shared<MeshBase>> m_meshes;
    /// VAO for internal and static usage, like when drawing background or debug curves/planes.
    OpenGL::VertexArray m_vao_internal;
    /// Program pipeline for internal usage, consisting of statges from pre-defined shader only.
    OpenGL::ProgramPipeline m_pipeline_internal;
    /// Debug drawing -- RGB unit axes located at world origin
    OpenGL::Program m_debug_axes;

    void aux_import_shader(const DynamicFile& file, const std::string& tag);
    void aux_import_image(const DynamicFile& file, const std::string& tag);
    void aux_import_geometry(const DynamicFile& file, const std::string& tag);
    void aux_import_dependency(const DynamicFile& path, const std::string& tag);
};

extern std::unique_ptr<Sandbox> sandbox;

