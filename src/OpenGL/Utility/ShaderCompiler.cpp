#include "OpenGL/Utility/ShaderCompiler.hpp"
#include "Debug.hpp"
#include "Log.hpp"

#include <fstream>


namespace OpenGL {

ShaderSource::ShaderSource(const std::string& file)
{
    std::ifstream ifs;
    ifs.open(file.c_str());
    if (!ifs) {
        Log::e("Failed to open {}", file);
        return;
    }
    type = shaderTypeOfSuffix(file.substr(file.rfind('.') + 1));
    source = std::string(std::istreambuf_iterator<char>(ifs),
                         std::istreambuf_iterator<char>());
    assert(!source.empty());

}

shared_shader
ShaderCompiler::compile(const OpenGL::ShaderSource& source,
                        bool force_compile)
{
    // lookup cache
    auto find = m_cache.find(source);
    if (find != m_cache.end()) {
        if (force_compile) {
            m_cache.erase(find);
        } else {
            return find->second;
        }
    }
    // check shader type
    if (source.type == GL_UNKNOWN_SHADER) {
        Log::e("Can't determine shader type");
    }
    auto&&[it, result] = m_cache.emplace(source,
                                        std::make_shared<Shader>(source.type));
    if (!result) {
        Log::e("Failed to emplace shader into cache");
    }
    shared_shader ret = it->second;
    ret->source(source.source.c_str());
    ret->compile();
    return ret;
}

} // namespace OpenGL
