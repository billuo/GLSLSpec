/**
 * @file Texture.hpp
 * @author lz1008 461652354@qq.com
 */
#pragma once

#include <OpenGL/Object/Object.hpp>


namespace OpenGL {

class Texture : public Object {
    static auto& pool()
    {
        static auto&& singleton = make_pool(glGenTextures, glDeleteTextures);
        return singleton;
    }

  public:
    Texture() : Object(pool().get())
    {}

    ~Texture()
    {
        pool().put(std::move(m_name));
    }

  private:
};

} // namespace OpenGL

