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

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Sources and executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# External dependencies
find_package(SDL2 REQUIRED)

# Include directories
include_directories(
    include
    ${SDL2_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    private ${SDL2_LIBRARIES}
)
