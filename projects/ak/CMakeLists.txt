set(target_name "ak")
set(obj_name "${target_name}_obj")
set(targ_path "${CMAKE_SOURCE_DIR}/projects/${target_name}")

cm_target_get_files_pub(${target_name} ${targ_path} files_pub)
cm_target_get_files_priv(${target_name} ${targ_path} files_priv)

cm_target_get_includes_pub(${target_name} ${targ_path} inc_pub)
cm_target_get_includes_priv(${target_name} ${targ_path} inc_priv)

cm_target_get_defines_pub(${target_name} ${targ_path} defs_pub)
cm_target_get_defines_priv(${target_name} ${targ_path} defs_priv)
cm_target_get_defines_shared_priv(${target_name} ${targ_path} defs_shared)


add_library(${obj_name} OBJECT
)
target_sources(${obj_name}
	PUBLIC
	"${files_pub}"
	PRIVATE
	"${files_priv}"
)
target_include_directories(${obj_name}
	PUBLIC
	"${inc_pub}"
	PRIVATE 
	"${inc_priv}"
)
target_compile_definitions(${obj_name}
	PUBLIC
	"${defs_pub}"
	PRIVATE
	"${defs_priv}"
	"${defs_shared}"
)

#--- math ---#
target_link_libraries(${obj_name} PRIVATE m)

#--- posix ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "posix" files_posix)
target_sources(${obj_name}
	PRIVATE
	${files_posix}
)

#--- x11 ---#
find_package(X11 REQUIRED)

cm_target_get_files_impl_priv(${target_name} ${targ_path} "x11" files_x11)
target_sources(${obj_name}
	PRIVATE
	${files_x11}
)
target_include_directories(${obj_name} PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(${obj_name} PRIVATE ${X11_LIBRARIES})


#--- opengl ---#
find_package(OpenGL REQUIRED)
cm_target_get_files_impl_priv(${target_name} ${targ_path} "opengl" files_opengl)
LIST(APPEND files_opengl "${CMAKE_SOURCE_DIR}/thirdparty/glad/src/glad.c")
target_sources(${obj_name}
	PRIVATE
	${files_opengl}
)
target_include_directories(${obj_name} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/glad/include/")
target_link_libraries(${obj_name} PRIVATE OpenGL::GL)

#--- opengl x11 ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "opengl_x11" files_opengl_x11)
target_sources(${obj_name}
	PRIVATE
	${files_opengl_x11}
)

#--- stb ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "stb" files_stb)
list(APPEND files_stb "${CMAKE_SOURCE_DIR}/thirdparty/stb/stb_image.h")
target_sources(${obj_name}
	PRIVATE
	${files_stb}
)
target_include_directories(${obj_name} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/")

#--- target ---#
add_library(ak SHARED
)
set_target_properties(${target_name} 
	PROPERTIES 
	LINKER_LANGUAGE C
)
target_link_libraries(${target_name} PUBLIC ${obj_name})

#--- tests ---#
add_subdirectory("./tests")
