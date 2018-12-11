#pragma once

#include "Scene/Camera.hpp"
#include "Watcher.hpp"
#include "OpenGL/Introspection/Introspector.hpp"
#include "OpenGL/Object/Buffer.hpp"
#include "OpenGL/Object/ProgramPipeline.hpp"
#include "OpenGL/Object/VertexArray.hpp"


class Sandbox {
  public:
    Sandbox();

    void on_update(const DynamicFile& file);

    void update();

    void render();

    Scene::Camera camera{glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3()};

  private:

    std::set<DynamicFile> m_updated;
    std::mutex mutex_updated;

    OpenGL::ProgramPipeline m_pipeline;
    std::unique_ptr<OpenGL::Introspector> m_introspectors[OpenGL::MaxShaderStage]{};
    OpenGL::VertexArray m_vao;

    void import(const DynamicFile& file);
    void aux_import_shader(const DynamicFile& file, const std::string& tag);
    void aux_import_image(const DynamicFile& path, const std::string& tag);
    void aux_import_geometry(const DynamicFile& path, const std::string& tag);
    void aux_import_dependency(const DynamicFile& path, const std::string& tag);
};

extern std::unique_ptr<Sandbox> sandbox;

