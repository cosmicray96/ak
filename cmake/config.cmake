#--- c ---#
macro(cmi_c_set)
	set(CMAKE_C_STANDARD "99")
	set(CMAKE_C_STANDARD_REQUIRED ON)
	set(CMAKE_C_EXTENSIONS OFF)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
	enable_testing()
endmacro()

#--- proj root ---#
set(cm_proj_root_dir "")
macro(cmi_proj_root_dir_set)
	get_filename_component(cm_proj_root_dir "${CMAKE_SOURCE_DIR}" ABSOLUTE)
endmacro()

#--- output dir name ---#
set(cm_output_dir_suffix "")
macro(cm_output_dir_suffix_set name)
	set(cm_output_dir_suffix  ${name})
endmacro()

#--- dirs ---#
set(cm_bin_dir "")
set(cm_junk_dir "")
set(cm_tests_dir "")
set(cm_exe_dir "")
macro(cmi_output_dirs_set)

	set(suffix ${cm_output_dir_suffix})

	set(cm_bin_dir
		"${cm_proj_root_dir}/_bin/${CMAKE_BUILD_TYPE}_${suffix}"
	)
	set(cm_junk_dir "${cm_bin_dir}/_junk")
	set(cm_tests_dir "${cm_bin_dir}/_tests")
	set(cm_exe_dir "${cm_bin_dir}")

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${cm_exe_dir}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${cm_exe_dir}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${cm_junk_dir}")
endmacro()

#--- clangd ---#
macro(cmi_clangd_commands_json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${proj_root_dir}/compile_commands.json
    COMMENT "Copying compile_commands.json to project root"
    VERBATIM
)
endmacro()
