#pragma once

#include "Watcher.hpp"
#include "OpenGL/Object/VertexArray.hpp"
#include "OpenGL/Object/Buffer.hpp"
#include "OpenGL/Object/ProgramPipeline.hpp"


class Sandbox {
  public:
    void on_update(const DynamicFile& file);

    void update();

    void render();

  private:

    std::set<DynamicFile> m_updated;
    std::mutex mutex_updated;

    bool valid = true;
    OpenGL::ProgramPipeline m_pipeline{"sbpp"};
    OpenGL::VertexArray m_vao{"sbvao"};

    void import(const DynamicFile& file);
    void aux_import_shader(const DynamicFile& file, const std::string& tag);
    void aux_import_image(const DynamicFile& path, const std::string& tag);
    void aux_import_geometry(const DynamicFile& path, const std::string& tag);
    void aux_import_dependency(const DynamicFile& path, const std::string& tag);
};

extern std::unique_ptr<Sandbox> sandbox;

