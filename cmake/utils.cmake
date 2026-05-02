function(cmi_check_build_type)
	set(bt ${CMAKE_BUILD_TYPE})
	if(NOT bt)
		set(bt "Debug")
		message(STATUS "No build type specified, defaulting to Debug")
	endif()

	set(valid_build_types Debug Release MinSizeRel RelWithDebInfo)

	list(FIND valid_build_types "${bt}" bt_index)
	if(bt_index EQUAL -1)
		message(FATAL_ERROR
			"Invalid CMAKE_BUILD_TYPE: ${bt}\n"
			"Must be one of: ${valid_build_types}")
	endif()

		set(CMAKE_BUILD_TYPE "${bt}" PARENT_SCOPE)
endfunction()

