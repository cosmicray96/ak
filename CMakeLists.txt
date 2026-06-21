cmake_minimum_required(VERSION 3.28.3)
project(cmake_proj C)

include("./cmake/cm.cmake")

cm_c_set()
cm_output_dirs_set(opengl_posix_x11)
cm_clangd_commands_json()


add_subdirectory("${CMAKE_SOURCE_DIR}/projects/ak")
add_subdirectory("${CMAKE_SOURCE_DIR}/projects/main")
