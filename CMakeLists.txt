cmake_minimum_required(VERSION 3.28.3)
project(cmake_proj C)

include("./cmake/cm.cmake")
cm_begin()
cm_output_dir_suffix_set(opengl_posix_x11)
cm_end()


add_subdirectory("${cm_proj_root_dir}/projects/ak")
add_subdirectory("${cm_proj_root_dir}/projects/main")
