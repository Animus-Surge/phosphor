### Phosphor CMakeLists.txt
# Author: Surge
# Version: 0.1.3
# Description: An SDL based 2D/3D game engine
# License: MIT

# Project setup
cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts")

project(phosphor
    VERSION 0.1.0
    DESCRIPTION "An SDL based 2D/3D game engine"
    LANGUAGES CXX)

#execute_process(
#        COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/scanSources.py
#        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#)

# Variables
set(PHOSPHOR_BUILD_TESTS ON) # TODO: check if these variables are defined already
set(PHOSPHOR_BUILD_EDITOR OFF)
set(PHOSPHOR_BUILD_EXAMPLES OFF) # No examples yet

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

execute_process(
        COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/scanSources.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include(${CMAKE_SOURCE_DIR}/scripts/sources.cmake)

# Dependencies
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/scanSources.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Testing project
if(PHOSPHOR_BUILD_TESTS)
    add_subdirectory(test)
endif()

# Editor project
#TODO

# Linking
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE spdlog::spdlog
    PRIVATE glm::glm
    PRIVATE $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main> $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    PRIVATE imgui::imgui
    PRIVATE Vulkan::Vulkan
    PRIVATE OpenGL::GL
    PRIVATE GLEW::GLEW
    PRIVATE assimp::assimp
)
