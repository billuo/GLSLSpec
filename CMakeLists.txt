cmake_minimum_required(VERSION 3.12)
project(OpenGL-lab)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/OpenGL)

add_executable(Main
        include/OpenGL/Buffer.hpp
        include/OpenGL/Common.hpp
        include/OpenGL/Introspection.hpp
        include/OpenGL/Object.hpp
        include/OpenGL/Program.hpp
        include/OpenGL/ProgramInput.hpp
        include/OpenGL/ProgramOutput.hpp
        include/OpenGL/ProgramPipeline.hpp
        include/OpenGL/Resource.hpp
        include/OpenGL/Shader.hpp
        include/OpenGL/SubroutineUniform.hpp
        include/OpenGL/Uniform.hpp
        include/OpenGL/UniformBlock.hpp
        include/Debug.hpp
        include/glutCallback.hpp
        include/Math.hpp
        include/Mesh.hpp
        include/Model.hpp
        include/Render.hpp
        include/Utility.hpp
        shaders/axes.frag
        shaders/axes.vert
        shaders/shader.frag
        shaders/shader.geom
        shaders/shader.vert
        src/OpenGL/Buffer.cpp
        src/OpenGL/Common.cpp
        src/OpenGL/Introspection.cpp
        src/OpenGL/Program.cpp
        src/OpenGL/Resource.cpp
        src/OpenGL/Shader.cpp
        src/Debug.cpp
        src/glutCallback.cpp
        src/main.cpp
        src/Mesh.cpp
        src/Render.cpp
        src/Utility.cpp include/OpenGL/Texture.hpp)

target_compile_definitions(Main PUBLIC $<$<CONFIG:Debug>:DEBUG_BUILD>)
target_compile_options(Main PUBLIC $<$<CONFIG:Release>:-O2>)
target_compile_options(Main PUBLIC $<$<CONFIG:Debug>:-O0 -g>)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found at:\n${OPENGL_INCLUDE_DIR};\n${OPENGL_LIBRARIES};")
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Main ${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
message(STATUS "glut found at:\n${GLUT_INCLUDE_DIR};\n${GLUT_LIBRARIES};")
include_directories(${GLUT_INCLUDE_DIR})
target_link_libraries(Main ${GLUT_LIBRARIES})

find_package(GLEW REQUIRED)
message(STATUS "glew found at:\n${GLEW_INCLUDE_DIRS};\n${GLEW_LIBRARIES};")
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(Main ${GLEW_LIBRARIES})

file(COPY shaders DESTINATION ./)
file(COPY resources DESTINATION ./)
