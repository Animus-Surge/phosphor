### Phosphor CMakeLists.txt
# Author: Surge
# Version: 0.1.3
# Description: An SDL based 2D/3D game engine
# License: MIT

cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")

project(phosphor
    VERSION 0.1.3
    DESCRIPTION "An SDL based 2D/3D game engine"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Dependencies
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Testing project
add_subdirectory(test)

# Linking
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE spdlog::spdlog
    PRIVATE glm::glm
    PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    PRIVATE imgui::imgui
    PRIVATE Vulkan::Vulkan
)
