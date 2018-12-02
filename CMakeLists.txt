cmake_minimum_required(VERSION 3.12)
project(OpenGL-lab)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
include(cotire/cotire)

include_directories(include)
include_directories(3rdparty)

add_library(MainLib
        src/OpenGL/Object/Buffer.cpp
        src/OpenGL/Common.cpp
        src/OpenGL/Utility/Debug.cpp
        src/OpenGL/Introspection/Introspection.cpp
        src/OpenGL/Constants.cpp
        src/OpenGL/Introspection/Introspector.cpp
        src/OpenGL/Introspection/Resource.cpp
        src/OpenGL/Object/Object.cpp
        src/OpenGL/Object/Program.cpp
        src/OpenGL/Object/Shader.cpp
        src/OpenGL/Utility/ShaderCompiler.cpp
        src/glutCallback.cpp
        src/Debug.cpp
        src/Log.cpp
        src/Mesh.cpp
        src/Render.cpp
        src/Transform.cpp
        src/Utility.cpp src/OpenGL/Introspection/UniformBlock.cpp src/OpenGL/Introspection/ProgramInput.cpp src/OpenGL/Introspection/ProgramOutput.cpp src/OpenGL/Introspection/SubroutineUniform.cpp src/OpenGL/Introspection/Uniform.cpp)

target_compile_definitions(MainLib PUBLIC $<$<CONFIG:Debug>:DEBUG_BUILD>)
target_compile_options(MainLib PUBLIC $<$<CONFIG:Release>:-O2>)
target_compile_options(MainLib PUBLIC $<$<CONFIG:Debug>:-O0 -g>)

#set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found at:\n${OPENGL_INCLUDE_DIR};\n${OPENGL_LIBRARIES};")
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(MainLib ${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
message(STATUS "glut found at:\n${GLUT_INCLUDE_DIR};\n${GLUT_LIBRARIES};")
include_directories(${GLUT_INCLUDE_DIR})
target_link_libraries(MainLib ${GLUT_LIBRARIES})

find_package(GLEW REQUIRED)
message(STATUS "glew found at:\n${GLEW_INCLUDE_DIRS};\n${GLEW_LIBRARIES};")
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(MainLib ${GLEW_LIBRARIES})

add_subdirectory(3rdparty/assimp)
target_link_libraries(MainLib assimp)

add_executable(Main src/main.cpp)
add_dependencies(Main MainLib)
target_compile_definitions(Main PUBLIC $<$<CONFIG:Debug>:DEBUG_BUILD>)
target_compile_options(Main PUBLIC $<$<CONFIG:Release>:-O2>)
target_compile_options(Main PUBLIC $<$<CONFIG:Debug>:-O0 -g>)
target_link_libraries(Main MainLib)

add_executable(test EXCLUDE_FROM_ALL
        test/catch.cpp
        test/test001.cpp)
add_dependencies(test MainLib)
target_link_libraries(test MainLib)

file(COPY shaders DESTINATION ./)
file(COPY resources DESTINATION ./)

option(UNITY_BUILD "Use cotire to enable unity build, speeding up compilation.")
if (UNITY_BUILD)
    cotire(MainLib)
    cotire(test)
endif ()
