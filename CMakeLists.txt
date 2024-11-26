# Phosphor CMakeLists.txt
#

cmake_minimum_required(VERSION 3.13)
include(ExternalProject)

# Clangd config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Module path config
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")

project(phosphor
    VERSION 0.1.0
    DESCRIPTION "An SDL2-based game engine"
    LANGUAGES CXX
)

#FetchContent
include(FetchContent)

#Render system  
add_compile_definitions(PHOSPHOR_SDL)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# External dependencies
find_package(SDL2 REQUIRED) # SDL2 - Windowing and input (alternative to GLFW/GLUT)
find_package(SDL2_image REQUIRED) # SDL2_image - Image loading
find_package(OpenGL REQUIRED) # OpenGL - Graphics backend
find_package(GLEW REQUIRED) # GLEW - OpenGL Extension Wrangler

#nlohmann/json - JSON parser
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(external/nlohmann_json)

#glm - OpenGL Mathematics
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)
FetchContent_MakeAvailable(glm)

add_subdirectory(test)

# Include directories
include_directories(
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/external/nlohmann_json/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${SDL2_IMAGE_LIBRARIES}
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE ${GLEW_LIBRARIES}
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE glm::glm
)
