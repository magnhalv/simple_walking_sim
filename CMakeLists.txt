cmake_minimum_required(VERSION 3.12)

project(RenderingCookbook CXX C)

option(BUILD_WITH_EASY_PROFILER "Enable EasyProfiler usage" ON)
option(BUILD_WITH_OPTICK "Enable Optick usage" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(WIN32)
	set(PYTHON_EXECUTABLE "python")
else()
	set(PYTHON_EXECUTABLE "python3")
endif()

if((NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/.bootstrap.json") OR ("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" IS_NEWER_THAN "${CMAKE_CURRENT_SOURCE_DIR}/deps/.bootstrap.json"))
	execute_process(
		COMMAND ${PYTHON_EXECUTABLE} "bootstrap.py"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE bootstrap_result
	)
	if (NOT (${bootstrap_result} EQUAL 0))
		message(FATAL_ERROR "Cannot run bootstrap.py")
	endif()
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")

set(EASY_PROFILER_NO_GUI ON CACHE BOOL "")
set(EASY_PROFILER_NO_SAMPLES ON CACHE BOOL "")

set(ASSIMP_NO_EXPORT ON CACHE BOOL "")
set(ASSIMP_BUILD_DRACO OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "")

set(MESHOPT_BUILD_DEMO OFF CACHE BOOL "")
set(MESHOPT_BUILD_TOOLS OFF CACHE BOOL "")
set(MESHOPT_BUILD_SHARED_LIBS OFF CACHE BOOL "")

add_subdirectory(deps/src/etc2comp)

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#include_directories(deps/src/vulkan/include)

add_subdirectory(deps/src/glfw)
if(BUILD_WITH_OPTICK)
add_subdirectory(deps/src/optick)
endif()
add_subdirectory(deps/src/easy_sources)
add_subdirectory(deps/src/assimp)
add_subdirectory(deps/src/meshoptimizer)
add_subdirectory(deps/src/volk)

set(ENABLE_GLSLANG_BINARIES  OFF  CACHE BOOL "")
set(ENABLE_HLSL              OFF  CACHE BOOL "")
set(ENABLE_CTEST             OFF  CACHE BOOL "")
set(ENABLE_OPT               OFF  CACHE BOOL "")
set(SKIP_GLSLANG_INSTALL     ON   CACHE BOOL "")
add_subdirectory(deps/src/glslang)

include_directories(.)
include_directories(deps/src)
include_directories(deps/src/glfw/include)
include_directories(deps/src/glm)
include_directories(deps/src/gli)
include_directories(deps/glad/include)
include_directories(deps/src/assimp/include)
include_directories(deps/src/etc2comp/EtcLib/Etc)
include_directories(deps/src/etc2comp/EtcLib/EtcCodec)
include_directories(deps/src/glslang/glslang/Include)
include_directories(deps/src/taskflow)
include_directories(deps/src/rapidjson/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/deps/src/assimp/include)
include_directories(deps/src/stb)
include_directories(deps/src/glslang)
include_directories(deps/src/glslang/StandAlone)
include_directories(deps/src/glslang/src)
include_directories(deps/src/bullet/src)
include_directories(deps/src/ImGuizmo/src)

add_subdirectory(deps/cmake/bullet)
add_subdirectory(deps/cmake/ImGuizmo)

# GLAD
include_directories(deps/glad/include)
set(GLAD_SOURCES deps/glad/src/gl.c deps/glad/src/vulkan.c)
if(WIN32)
	set(GLAD_SOURCES ${GLAD_SOURCES} deps/glad/src/wgl.c)
endif()

add_library(glad ${GLAD_SOURCES})

# shared utils code for GL and Vulkan
# add_subdirectory(shared)

#target_sources(SharedUtils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/deps/src/glslang/StandAlone/ResourceLimits.cpp)

# set the project name
project(SimpleWalkingSim)

#set(RENDERER_SOURCE_FILES
		#${CMAKE_SOURCE_DIR}/src/renderer/index_buffer.cpp
		#${CMAKE_SOURCE_DIR}/src/renderer/draw.cpp
		#src/str_util.cpp src/str_util.h)

# add the executable
add_executable(SimpleWalkingSim ${CMAKE_SOURCE_DIR}/src/main.cpp
		${CMAKE_SOURCE_DIR}/src/utils.cpp
		${CMAKE_SOURCE_DIR}/src/gl_shader.cpp
		${CMAKE_SOURCE_DIR}/src/renderer.cpp
		${GLAD_SOURCES}
		)

target_include_directories(SimpleWalkingSim PRIVATE
		${CMAKE_SOURCE_DIR}/src
		)

target_link_libraries(SimpleWalkingSim
		assimp
		opengl32
		)
# TODO: Find a way to detect windows
set_property(TARGET SimpleWalkingSim PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
target_compile_features(SimpleWalkingSim PRIVATE cxx_std_20)

add_compile_definitions(GAME_SLOW=1)




