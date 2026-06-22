#--- check ---#
macro(cm_check)
if(NOT CMAKE_BUILD_TYPE)
	message("CMAKE_BUILD_TYPE not set, defaulting to Debug.")
	set(CMAKE_BUILD_TYPE Debug)
endif()
endmacro()

#--- c ---#
macro(cm_c_set)
	set(CMAKE_C_STANDARD "99")
	set(CMAKE_C_STANDARD_REQUIRED ON)
	set(CMAKE_C_EXTENSIONS OFF)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

	set(CMAKE_INSTALL_RPATH "\$ORIGIN")
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

	#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	#add_link_options("-Wl,-rpath,\$ORIGIN")

	enable_testing()
endmacro()

#--- dirs ---#
set(cm_bin_dir "")
set(cm_junk_dir "")
set(cm_tests_dir "")
set(cm_exe_dir "")
macro(cm_output_dirs_set suffix)

	set(cm_bin_dir
		"${CMAKE_SOURCE_DIR}/_bin/${suffix}"
	)
	set(cm_junk_dir "${cm_bin_dir}/_junk")
	set(cm_tests_dir "${cm_bin_dir}/_tests")
	set(cm_exe_dir "${cm_bin_dir}")

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${cm_exe_dir}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${cm_exe_dir}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${cm_junk_dir}")
endmacro()

#--- clangd ---#
macro(cm_clangd_commands_json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
				${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to project root"
    VERBATIM
)
endmacro()
