/**
 * @File Misc.cpp
 * @author Zhen Luo 461652354@qq.com
 */
#include <Utility/Misc.hpp>


namespace {
const char*
demangle_error_msg(int err)
{
    switch (err) {
        case -1:
            return "Memory allocation failed";
        case -2:
            return "Invalid mangled name";
        case -3:
            return "Invalid arguments";
        default:
            return "Unknown";
    }
}
} // namespace

#if !CXX_MSVC

#include <cxxabi.h>


char*
SafeDemangle(const char* mangled_name, char* output_buffer, size_t* length)
{
    int status;
    char* ret =
            abi::__cxa_demangle(mangled_name, output_buffer, length, &status);
    if (status != 0) {
        Log::e("Failed to demangle name '{}':{}\n",
               mangled_name,
               demangle_error_msg(status));
    }
    return ret;
}

#endif
