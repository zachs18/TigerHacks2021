cmake_minimum_required(VERSION 3.16)
project(asteroids_clone)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
#pkg_search_module(GLEW REQUIRED glfw3)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

add_executable(asteroids_clone
    src/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/vertex_shader_source.o
    ${CMAKE_CURRENT_BINARY_DIR}/fragment_shader_source.o
)

#target_include_directories(asteroids_clone ${GLFW_INCLUDE_DIRS}) # doesn't work because its empty
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(asteroids_clone ${GLFW_LIBRARIES})
target_link_libraries(asteroids_clone GL)
target_link_libraries(asteroids_clone ${GLFW_STATIC_LIBRARIES})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vertex_shader_source.o
    COMMAND objcopy -I binary ${CMAKE_CURRENT_SOURCE_DIR}/src/vertex_shader.glsl -O elf64-x86-64 vertex_shader_source_verbose.o
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/rename_symbols.sh vertex_shader_source_verbose.o vertex_shader_source.o vertex_shader_source
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/vertex_shader.glsl
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rename_symbols.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fragment_shader_source.o
    COMMAND objcopy -I binary ${CMAKE_CURRENT_SOURCE_DIR}/src/fragment_shader.glsl -O elf64-x86-64 fragment_shader_source_verbose.o
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/rename_symbols.sh fragment_shader_source_verbose.o fragment_shader_source.o fragment_shader_source
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/fragment_shader.glsl
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rename_symbols.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
