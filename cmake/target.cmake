function(cmi_impl_add target_name)
	set(targ_path "${cm_proj_root_dir}/projects/${target_name}")
	foreach(impl IN LISTS cm_impls)

		file(GLOB_RECURSE files_pub
			"${targ_path}/inc/${target_name}_${impl}/*.h"
		)
		file(GLOB_RECURSE files_priv
			"${targ_path}/src/${target_name}_${impl}/*.h"
			"${targ_path}/src/${target_name}_${impl}/*.c"
		)
		target_sources(${target_name}
			PUBLIC
			${files_pub}
			PRIVATE
			${files_priv}
		)

	endforeach()

	foreach(impl_target IN LISTS cm_impl_targets)
		target_link_libraries(${target_name} PRIVATE
		${impl_target}
		)
	endforeach()

endfunction()

function(cm_target_add_exe target_name)
	set(targ_path "${cm_proj_root_dir}/projects/${target_name}")
	file(GLOB_RECURSE files_priv
		"${targ_path}/src/${target_name}/*.h"
		"${targ_path}/src/${target_name}/*.c"
	)
	add_executable(${target_name})
	target_sources(${target_name}
		PRIVATE
		${files_priv}
	)
	target_include_directories(${target_name}
		PRIVATE 
		"${targ_path}/src/"
	)
	target_compile_definitions(${target_name}
		PUBLIC
		"${target_name}_${CMAKE_BUILD_TYPE}"
	)

target_link_libraries(${target_name} PRIVATE m)
	cmi_impl_add(${target_name})
endfunction()

function(cm_target_add_shared target_name)
	set(targ_path "${cm_proj_root_dir}/projects/${target_name}")
	file(GLOB_RECURSE files_pub
		"${targ_path}/inc/${target_name}/*.h"
	)
	file(GLOB_RECURSE files_priv
		"${targ_path}/src/${target_name}/*.h"
		"${targ_path}/src/${target_name}/*.c"
	)
	add_library(${target_name} SHARED
	)
	target_sources(${target_name}
		PUBLIC
		${files_pub}
		PRIVATE
		${files_priv}
	)
	target_include_directories(${target_name}
		PUBLIC
		"${targ_path}/inc/"
		PRIVATE 
		"${targ_path}/src/"
	)
	target_compile_definitions(${target_name}
		PUBLIC
		"${target_name}_${CMAKE_BUILD_TYPE}"
		"${target_name}_shared"
		PRIVATE
		"${target_name}_inside"
	)

target_link_libraries(${target_name} PRIVATE m)
	cmi_impl_add(${target_name})
endfunction()

