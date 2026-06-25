cmake_minimum_required(VERSION 3.28.3)
project(cmake_proj C)

include("./cmake/cm.cmake")

cm_check()
cm_c_set()
cm_clangd_commands_json()
if(cm_plat STREQUAL "linux")
cm_output_dirs_set("${CMAKE_BUILD_TYPE}_opengl_posix_x11")
elseif(cm_plat STREQUAL "android")
cm_output_dirs_set("${CMAKE_BUILD_TYPE}_android")
endif()
cm_print()


add_subdirectory("${CMAKE_SOURCE_DIR}/projects/ak")

if(cm_plat STREQUAL "linux")
add_subdirectory("${CMAKE_SOURCE_DIR}/projects/main")
elseif(cm_plat STREQUAL "android")
add_subdirectory("${CMAKE_SOURCE_DIR}/projects/main_android")
endif()
