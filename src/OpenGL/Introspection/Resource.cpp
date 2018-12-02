#include "OpenGL/Introspection/Resource.hpp"
#include <sstream>


namespace OpenGL {

std::string
Resource::referenced_stages(const GLint referenced[MaxShaderStage])
{
    std::stringstream ss;
    ss << "Referenced by:";

#define STAGE(stage) if (referenced[stage]) ss << "\n\t" << #stage;
    STAGE(VertexShader);
    STAGE(TessellationControlShader);
    STAGE(TessellationEvaluationShader);
    STAGE(GeometryShader);
    STAGE(FragmentShader);
    STAGE(ComputeShader);
#undef STAGE

    return ss.str();
}

std::ostream&
operator<<(std::ostream& os, const Resource& resource)
{ return os << "index=" << resource.index << ", name=" << resource.name; }

} // namespace OpenGL
