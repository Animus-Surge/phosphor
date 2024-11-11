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
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED) # TODO: remove this dependency in favor of a custom font rendering system

add_subdirectory(test)

# Include directories
include_directories(
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${SDL2_IMAGE_LIBRARIES}
    PRIVATE ${SDL2_TTF_LIBRARIES}
)
