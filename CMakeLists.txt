# Phosphor CMakeLists.txt
#

cmake_minimum_required(VERSION 3.13)
include(ExternalProject)

# Clangd config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Module path config
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts/cmake/")

project(phosphor
    VERSION 0.1.1
    DESCRIPTION "An SDL2-based game engine"
    LANGUAGES CXX
)

#TODO: vulkan SDK

#FetchContent
include(FetchContent)

#Render system  
add_compile_definitions(PHOSPHOR_SDL)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

#Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG f401021d5a5d56fe2304056c391e78f81c8d4b8f
)
FetchContent_MakeAvailable(imgui)

#Vulkan
set(VULKAN_SDK_PATH ${CMAKE_SOURCE_DIR}/external/vulkan)
set(VULKAN_LIBRARY ${VULKAN_SDK_PATH}/x86_64/lib/libvulkan.so)
set(VULKAN_INCLUDE_DIR ${VULKAN_SDK_PATH}/x86_64/include)


#spdlog - Logging
#set(SPDLOG_FMT_EXTERNAL ON)
set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)
add_subdirectory(external/spdlog)

#ImGui target
add_library(imgui_shared SHARED
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_subdirectory(test)

# Symlink test/resources to build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND rm -rf $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources # Remove the old symlink
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/test/resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

# Include directories
include_directories(
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${VULKAN_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/external/nlohmann_json/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${CMAKE_SOURCE_DIR}/external/spdlog/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE ${SDL2_LIBRARIES}
    PRIVATE ${SDL2_IMAGE_LIBRARIES}
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE ${GLEW_LIBRARIES}
    PRIVATE ${VULKAN_LIBRARY}
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE glm::glm
    PRIVATE imgui_shared
    PRIVATE spdlog::spdlog
)
