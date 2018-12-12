/**
 * @File Sandbox.hpp
 * @author Zhen Luo 461652354@qq.com
 */
#pragma once

#include <Mesh.hpp>
#include <Scene/Camera.hpp>
#include <Watcher.hpp>
#include <OpenGL/Introspection/Introspector.hpp>
#include <OpenGL/Object/Buffer.hpp>
#include <OpenGL/Object/ProgramPipeline.hpp>
#include <OpenGL/Object/VertexArray.hpp>


class Sandbox {
  public:
    Sandbox();

    void on_update(const DynamicFile& file);

    void update();

    void render();

    void render_debug() const;

    Scene::Camera camera{glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3()};

  private:

    std::set<DynamicFile> m_updated;
    std::mutex mutex_updated;

    OpenGL::ProgramPipeline m_pipeline;
    std::unique_ptr<OpenGL::Introspector> m_introspectors[OpenGL::MaxShaderStage]{};
    std::vector<Mesh> m_meshes;

    OpenGL::VertexArray m_vao_debug;
    OpenGL::ProgramPipeline m_pipeline_debug;

    void import(const DynamicFile& file);
    void aux_import_shader(const DynamicFile& file, const std::string& tag);
    void aux_import_image(const DynamicFile& file, const std::string& tag);
    void aux_import_geometry(const DynamicFile& file, const std::string& tag);
    void aux_import_dependency(const DynamicFile& path, const std::string& tag);
};

extern std::unique_ptr<Sandbox> sandbox;

