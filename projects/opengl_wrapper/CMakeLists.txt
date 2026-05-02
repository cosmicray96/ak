find_package(OpenGL REQUIRED)

set(target_name "opengl_wrapper")

add_library(${target_name} STATIC ./src/glad.c)
target_include_directories(${target_name} PUBLIC ./inc)
target_link_libraries(${target_name} INTERFACE OpenGL::GL)
