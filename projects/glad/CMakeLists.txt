find_package(OpenGL REQUIRED)

set(target_name "glad")

add_library(${target_name} STATIC ./src/glad.c)
target_include_directories(${target_name} PUBLIC ./inc)
target_link_libraries(${target_name} INTERFACE OpenGL::GL)
