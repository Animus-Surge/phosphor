
set(BINARY_NAME phosphor_editor)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${BINARY_NAME} ${SOURCES})

include_directories(../include)
target_link_libraries(${BINARY_NAME} PRIVATE phosphor)
