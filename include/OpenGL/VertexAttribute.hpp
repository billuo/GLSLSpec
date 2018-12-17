#pragma once

#include <OpenGL/Common.hpp>

#include <string>


namespace OpenGL {

/// Define a single vertex attribute of on <B>server</B> side.
/// @details Contains information for use in glVertexAttribFormat()
struct VertexAttribute {
    /// Enumerating the only attributes this application provide to shaders.
    enum class Usage {
        Position,
        Color,
        Normal,
        TexCoord,
        Tangent,
        Other,
        Max,
    };

    /// @note The usage for this attribute is the only argument that must be specified,
    /// due to the fixed interface this application exposes to shader program (for now).
    explicit VertexAttribute(Usage usage, const std::string& name = "");

    /// User defined type of usage.
    Usage usage{Usage::Max};
    /// Name of attribute in shader used to find location, etc.
    std::string name{};
    /// Number of components we shall provide.
    GLint size{-1};
    /// Data type of each component in shader.
    GLenum type{GL_FALSE};
    /// If GL_TRUE, integer type will be normalized to float in range [-1,1] or [0,1] based on its signedness.
    /// @note Ignored when type is float type.
    GLboolean normalized{GL_FALSE};
    /// Relative offset of the first element of such attribute to the beginning of buffer supplying the data.
    /// @note It's only relevant if an interleaved approach is taken in the VertexBuffer. Otherwise leave it be 0.
    GLuint relative_offset{0};
};

} // namespace OpenGL
