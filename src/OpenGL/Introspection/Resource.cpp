/**
 * @File Resource.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <OpenGL/Introspection/Resource.hpp>
#include <sstream>


namespace OpenGL {

std::string
Resource::referenced_stages(const GLint referenced[MaxShaderStage])
{
    std::stringstream ss;
    ss << "Referenced by:";

#define STAGE(stage) if (referenced[underlying_cast(ShaderStage::stage)]) ss << "\n\t" << #stage;
    STAGE(Vertex);
    STAGE(TessellationControl);
    STAGE(TessellationEvaluation);
    STAGE(Geometry);
    STAGE(Fragment);
    STAGE(Compute);
#undef STAGE

    return ss.str();
}

std::ostream&
operator<<(std::ostream& os, const Resource& resource)
{ return os << "index=" << resource.index << ", name=" << resource.name; }

} // namespace OpenGL
