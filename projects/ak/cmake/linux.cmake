#--- pc ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "pc" files_pc)
target_sources(${obj_name}
	PRIVATE
	${files_pc}
)

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

