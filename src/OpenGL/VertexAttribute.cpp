#include <OpenGL/VertexAttribute.hpp>
#include <Utility/Enumeration.hpp>
#include <Utility/Misc.hpp>


using Usage = OpenGL::VertexAttribute::Usage;
DEFINE_ENUMERATION_DATABASE(Usage) {{Usage::Position, "position"},
                                    {Usage::Color,    "color"},
                                    {Usage::Normal,   "normal"},
                                    {Usage::TexCoord, "texture coordinate"},
                                    {Usage::Tangent,  "tangent"},
                                    {Usage::Other,    "others"}};

namespace OpenGL {

VertexAttribute::VertexAttribute(Usage _usage, const std::string& _name) : usage(_usage), name(_name)
{
    if (name.empty()) {
        name = "v_" + E<Usage>(usage).to_string();
    }
    switch (usage) {
        case Usage::Position:
        case Usage::Color:
        case Usage::Normal:
            size = 3;
            type = GL_FLOAT;
            break;
        case Usage::TexCoord:
            size = 2;
            type = GL_FLOAT;
            break;
        case Usage::Tangent:
        case Usage::Other:
            throw unimplemented("Vertex attribute default field value for " + E<Usage>(usage).to_string());
        case Usage::Max:
            break;
    }
}

} // namespace OpenGL
