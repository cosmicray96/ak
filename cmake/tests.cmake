function(cmi_impl_add_test target_name test_full_name)
	set(targ_path "${cm_proj_root_dir}/projects/${target_name}")
	foreach(impl IN LISTS cm_impls)

		file(GLOB_RECURSE files_pub
			"${targ_path}/inc/${target_name}_${impl}/*.h"
		)
		file(GLOB_RECURSE files_priv
			"${targ_path}/src/${target_name}_${impl}/*.h"
			"${targ_path}/src/${target_name}_${impl}/*.c"
		)
		target_sources(${test_full_name}
			PUBLIC
			${files_pub}
			PRIVATE
			${files_priv}
		)

	endforeach()
endfunction()

function(cm_test_add target_name test_name)
	set(test_full_name "${target_name}_${test_name}")
	set(targ_path "${cm_proj_root_dir}/projects/${target_name}")

	file(GLOB_RECURSE files
		"${targ_path}/inc/${target_name}/*.h"
		"${targ_path}/src/${target_name}/*.h"
		"${targ_path}/src/${target_name}/*.c"
	)

	add_executable(${test_full_name}
		${files}
		"${cm_proj_root_dir}/projects/${target_name}/tests/${test_name}.c"
	)
	
	target_include_directories(${test_full_name}
		PUBLIC
		"${targ_path}/inc/"
		PRIVATE 
		"${targ_path}/src/"
	)

	target_compile_definitions(${test_full_name}
		PUBLIC
		"${target_name}_${CMAKE_BUILD_TYPE}"
		"${target_name}_shared"
		PRIVATE
		"${target_name}_inside"
	)

	set_target_properties(${test_full_name} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${cm_tests_dir}/${target_name}"
	)

	cmi_impl_add_test(${target_name} ${test_full_name})
endfunction()
