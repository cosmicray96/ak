function(cm_target_get_files_impl_priv target_name targ_path impl o_files)
	file(GLOB_RECURSE files
		"${targ_path}/src/${target_name}_${impl}/*.h"
		"${targ_path}/src/${target_name}_${impl}/*.inc"
		"${targ_path}/src/${target_name}_${impl}/*.c"
	)
	set(${o_files} ${files} PARENT_SCOPE)
endfunction()


function(cm_target_get_files_pub target_name targ_path o_files)
	file(GLOB_RECURSE files
		"${targ_path}/inc/${target_name}/*.h"
		"${targ_path}/inc/${target_name}/*.inc"
	)
	set(${o_files} ${files} PARENT_SCOPE)
endfunction()

function(cm_target_get_files_priv target_name targ_path o_files)
	file(GLOB_RECURSE files
		"${targ_path}/src/${target_name}/*.h"
		"${targ_path}/src/${target_name}/*.inc"
		"${targ_path}/src/${target_name}/*.c"
	)
	set(${o_files} ${files} PARENT_SCOPE)
endfunction()

function(cm_target_get_includes_pub target_name targ_path o_dirs)
	set(${o_dirs}
		"${targ_path}/inc"
		PARENT_SCOPE
	)
endfunction()
function(cm_target_get_includes_priv target_name targ_path o_dirs)
	set(${o_dirs}
		"${targ_path}/src"
		PARENT_SCOPE
	)
endfunction()

function(cm_target_get_defines_pub target_name targ_path o_defs)
	set(defs
		"${target_name}_${CMAKE_BUILD_TYPE}"
	)
	set(${o_defs} ${defs} PARENT_SCOPE)
endfunction()

function(cm_target_get_defines_priv target_name targ_path o_defs)
	set(defs
		"${target_name}_inside"
	)
	set(${o_defs} ${defs} PARENT_SCOPE)
endfunction()

function(cm_target_get_defines_shared_priv target_name targ_path o_defs)
	set(defs
		"${target_name}_shared"
	)
	set(${o_defs} ${defs} PARENT_SCOPE)
endfunction()
