#include "Utility.hpp"
#include "Debug.hpp"

#if !CXX_MSVC
#include <cxxabi.h>
char* SafeDemangle(const char* mangled_name, char* output_buffer, size_t* length) {
    int status;
    char* ret = abi::__cxa_demangle(mangled_name, output_buffer, length, &status);
    if (status != 0) {
        const char* msg;
        switch (status) {
        case -1: msg = "Memory allocation failed"; break;
        case -2: msg = "Invalid mangled name"; break;
        case -3: msg = "Invalid arguments"; break;
        default: msg = "Unknown"; break;
        }
        DEBUG("Failed to demangle name '%s':%s\n", mangled_name, msg);
    }
    return ret;
}
#endif
