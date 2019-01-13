cmake_minimum_required(VERSION 3.12)
project(GLSLViewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -Wall)
set(OpenGL_GL_PREFERENCE LEGACY)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
include(cotire/cotire)

include_directories(include)
include_directories(3rdparty)

set(OPENGL_PROFILE core)
set(OPENGL_MAJOR_VERSION 4)
set(OPENGL_MINOR_VERSION 3)
set(OPENGL_VERSION ${OPENGL_MAJOR_VERSION}.${OPENGL_MINOR_VERSION})
set(OPENGL_EXTENSIONS_LIST
		GL_ARB_fragment_program
		GL_ARB_get_program_binary
		GL_ARB_texture_float
		GL_EXT_direct_state_access # not yet used, but likely will be
		)

string(REPLACE ";" "," OPENGL_EXTENSIONS "${OPENGL_EXTENSIONS_LIST}")

add_custom_command(
		OUTPUT
		${CMAKE_SOURCE_DIR}/3rdparty/glad/glad.c
		${CMAKE_SOURCE_DIR}/3rdparty/glad/glad.h
		${CMAKE_SOURCE_DIR}/3rdparty/glad/khrplatform.h
		COMMAND
		python3 -m glad --profile ${OPENGL_PROFILE} --out-path ${CMAKE_SOURCE_DIR}/3rdparty/glad/
		--api gl=${OPENGL_VERSION} --generator c --no-loader --local-files
		--extensions=${OPENGL_EXTENSIONS}
		COMMENT
		"Generating OpenGL loader with glad"
		VERBATIM)

set_source_files_properties(${CMAKE_SOURCE_DIR}/3rdparty/glad/glad.c PROPERTIES GENERATED TRUE)

add_custom_target(generated-glad DEPENDS
		${CMAKE_SOURCE_DIR}/3rdparty/glad/glad.c
		${CMAKE_SOURCE_DIR}/3rdparty/glad/glad.h
		${CMAKE_SOURCE_DIR}/3rdparty/glad/khrplatform.h)

#add_subdirectory(3rdparty/assimp)

add_library(third_party 3rdparty/glad/glad.c 3rdparty/tol/tol.cpp)
#link_libraries(third_party assimp)
add_dependencies(third_party generated-glad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter")

add_library(MainLib
		src/Console.cpp
		src/FileSystem.cpp
		src/Options.cpp
		src/Sandbox.cpp
		src/Watcher.cpp
		src/Window.cpp
		src/Math/Transform.cpp
        src/OpenGL/Common.cpp
        src/OpenGL/Constants.cpp
		src/OpenGL/Debug.cpp
        src/OpenGL/Introspection/Interface.cpp
        src/OpenGL/Introspection/Introspector.cpp
        src/OpenGL/Introspection/ProgramInput.cpp
        src/OpenGL/Introspection/ProgramOutput.cpp
        src/OpenGL/Introspection/Resource.cpp
        src/OpenGL/Introspection/SubroutineUniform.cpp
        src/OpenGL/Introspection/Uniform.cpp
        src/OpenGL/Introspection/UniformBlock.cpp
		src/OpenGL/Object/Buffer.cpp
		src/OpenGL/Object/Object.cpp
        src/OpenGL/Object/Program.cpp
		src/OpenGL/Object/ProgramPipeline.cpp
        src/OpenGL/Object/Shader.cpp
		src/Scene/Camera.cpp
		src/Scene/Node.cpp
		src/Utility/Log.cpp
		src/Utility/Misc.cpp
		src/OpenGL/VertexAttribute.cpp
		src/OpenGL/Object/Texture.cpp
		src/OpenGL/Object/Sampler.cpp
		src/OpenGL/Object/Framebuffer.cpp
		src/OpenGL/Object/RenderBuffer.cpp)

message(STATUS "Build target:${CMAKE_HOST_SYSTEM_NAME}")
set(HOST ${CMAKE_HOST_SYSTEM_NAME})
target_compile_definitions(MainLib PUBLIC APP_NAME=\"GLSLViewer\")
target_compile_definitions(MainLib PUBLIC APP_VERSION=\"0.3.0\")
target_compile_definitions(MainLib PUBLIC PLATFORM_NAME=\"${HOST}\")
target_compile_definitions(MainLib PUBLIC $<$<STREQUAL:${HOST},Linux>:PLATFORM_LINUX>)
target_compile_definitions(MainLib PUBLIC $<$<STREQUAL:${HOST},Windows>:PLATFORM_WINDOWS>)
target_compile_definitions(MainLib PUBLIC $<$<STREQUAL:${HOST},Darwin>:PLATFORM_OSX>)
target_compile_definitions(MainLib PUBLIC DEBUG_BUILD=$<CONFIG:Debug>)
target_compile_definitions(MainLib PUBLIC RELEASE_BUILD=$<CONFIG:Release>)
target_compile_definitions(MainLib PUBLIC $<$<CONFIG:Release>:NDEBUG=1>)
target_compile_definitions(MainLib PUBLIC OPENGL_MAJOR_VERSION=${OPENGL_MAJOR_VERSION})
target_compile_definitions(MainLib PUBLIC OPENGL_MINOR_VERSION=${OPENGL_MINOR_VERSION})
target_compile_options(MainLib PUBLIC $<$<CONFIG:Release>:-O3>)
target_compile_options(MainLib PUBLIC $<$<CONFIG:Debug>:-O0 -g>)

target_link_libraries(MainLib stdc++fs third_party)

find_package(Threads REQUIRED)
message(STATUS "Threads library:${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(MainLib ${CMAKE_THREAD_LIBS_INIT})

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found at:\n${OPENGL_INCLUDE_DIR};\n${OPENGL_LIBRARIES};")
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(MainLib ${OPENGL_LIBRARIES})

find_package(glfw3 3.2 REQUIRED)
target_link_libraries(MainLib glfw)

add_executable(GLSLSpec src/main.cpp)
add_dependencies(GLSLSpec MainLib)
target_compile_definitions(GLSLSpec PUBLIC $<$<CONFIG:Debug>:DEBUG_BUILD>)
target_compile_options(GLSLSpec PUBLIC $<$<CONFIG:Release>:-O2>)
target_compile_options(GLSLSpec PUBLIC $<$<CONFIG:Debug>:-O0 -g>)
target_link_libraries(GLSLSpec MainLib)

add_executable(test EXCLUDE_FROM_ALL
        test/catch.cpp
        test/test001.cpp)
add_dependencies(test MainLib)
target_link_libraries(test MainLib)

option(UNITY_BUILD "Use cotire to enable unity build, speeding up compilation.")
if (UNITY_BUILD)
    cotire(MainLib)
    cotire(test)
endif ()
