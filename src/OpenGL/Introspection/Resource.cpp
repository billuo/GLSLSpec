#include "OpenGL/Introspection/Resource.hpp"


namespace OpenGL {

void
Resource::dump_referenced(const GLint referenced[MaxShaderStage])
{
    fprintf(stderr, "Referenced by:\n");

#define STAGE(stage)       \
    if (referenced[stage]) \
    fprintf(stderr, "\t" #stage "\n")

    STAGE(VertexShader);
    STAGE(TessellationControlShader);
    STAGE(TessellationEvaluationShader);
    STAGE(GeometryShader);
    STAGE(FragmentShader);
    STAGE(ComputeShader);

#undef STAGE
    putc('\n', stderr);
}
} // namespace OpenGL
