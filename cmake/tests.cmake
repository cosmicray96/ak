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

	cmi_impl_add(${test_full_name})
endfunction()
