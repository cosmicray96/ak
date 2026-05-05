message("Cmake Template. Language: C, Version: 10")

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/target.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/utils.cmake")

macro(cm_begin)
	cmi_proj_root_dir_set()
	cmi_check_build_type()
	cmi_c_set()
endmacro()


macro(cm_end)
	cmi_output_dirs_set()
	cmi_clangd_commands_json()
endmacro()

