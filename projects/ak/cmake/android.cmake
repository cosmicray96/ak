#--- posix ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "posix" files_posix)
target_sources(${obj_name}
	PRIVATE
	${files_posix}
)

#--- android ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "android" files_android)
target_sources(${obj_name}
	PRIVATE
	${files_android}
)
target_include_directories(${obj_name} PRIVATE
	"${CMAKE_ANDROID_NDK}/sources/android/native_app_glue"
)
target_link_libraries(${obj_name} PRIVATE
	android
	EGL
	GLESv3
)

#--- opengl ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "opengl" files_opengl)
target_sources(${obj_name}
	PRIVATE
	${files_opengl}
)

#--- stb ---#
cm_target_get_files_impl_priv(${target_name} ${targ_path} "stb" files_stb)
list(APPEND files_stb "${CMAKE_SOURCE_DIR}/thirdparty/stb/stb_image.h")
target_sources(${obj_name}
	PRIVATE
	${files_stb}
)
target_include_directories(${obj_name} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/")

