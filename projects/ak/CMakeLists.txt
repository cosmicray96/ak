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

#--- platforms ---#
if(cm_plat STREQUAL "linux")
	include(cmake/linux.cmake)
elseif(cm_plat STREQUAL "android")
	include(cmake/android.cmake)
endif()

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
