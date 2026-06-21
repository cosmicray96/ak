set(target_name "ak")
set(obj_name "${target_name}_obj")
set(target_path "${CMAKE_SOURCE_DIR}/projects/ak")
set(tests "")


#list(APPEND tests "dbuff")
#list(APPEND tests "debug")
#list(APPEND tests "program")
#list(APPEND tests "hmn")
#list(APPEND tests "sla")
#list(APPEND tests "spa")
#list(APPEND tests "fixed")


foreach(t IN LISTS tests)

	set(test_full_name "${target_name}_${t}")
	add_executable(${test_full_name})

	target_sources(${test_full_name}
		PRIVATE
		"${CMAKE_SOURCE_DIR}/projects/${target_name}/tests/${t}.c"
	)

	target_link_libraries(${test_full_name}
		PRIVATE 
		${obj_name}
	)

	set_target_properties(${test_full_name} 
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY 
		"${cm_tests_dir}"
	)


endforeach()


