#pragma once

#include "../Common.hpp"
#include "../Constants.hpp"
#include "Utility/Misc.hpp"


namespace OpenGL {

struct Resource {
    GLint index = 0;
    std::string name;

    explicit Resource(GLint index) : index(index)
    {}

    Resource(GLint index, GLchar* name) : index(index), name(name)
    {}

    friend std::ostream& operator<<(std::ostream& os, const Resource& resource);

  protected:
    static std::string referenced_stages(const GLint referenced[MaxShaderStage]);
};

} // namespace OpenGL

