cmake_minimum_required(VERSION 3.1)
project(OpenGL-Lab)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(Main ${CMAKE_SOURCE_DIR}/src/main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)
set_property(TARGET Main PROPERTY CXX_STANDARD 98)
target_compile_options(Main PUBLIC -Wall PUBLIC -Wextra)
target_compile_options(Main PUBLIC $<$<CONFIG:Release>:-O2>)
target_compile_options(Main PUBLIC $<$<CONFIG:Debug>:-g>)
target_compile_options(Main PUBLIC -Wno-unused-local-typedefs)
target_compile_options(Main PUBLIC -Wno-unused-variable)
target_compile_options(Main PUBLIC -Wno-unused-parameter)
target_compile_options(Main PUBLIC -Wno-unused-function)

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found at:\n${OPENGL_INCLUDE_DIR};\n${OPENGL_LIBRARIES}")
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Main ${OPENGL_LIBRARIES})

find_package(GLUT REQUIRED)
message(STATUS "glut found at:\n${GLUT_INCLUDE_DIR};\n${GLUT_LIBRARIES}")
include_directories(${GLUT_INCLUDE_DIR})
target_link_libraries(Main ${GLUT_LIBRARIES})

find_package(GLEW REQUIRED)
message(STATUS "glew found at:\n${GLEW_INCLUDE_DIRS};\n${GLEW_LIBRARIES}")
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(Main ${GLEW_LIBRARIES})

set(Main_sources
    Debug.cpp
    OpenGL/Common.cpp
    OpenGL/Program.cpp
    OpenGL/Shader.cpp
)
foreach(src ${Main_sources})
    target_sources(Main PUBLIC ${CMAKE_SOURCE_DIR}/src/${src})
endforeach()

file(COPY shaders DESTINATION ./)
file(COPY resources DESTINATION ./)
