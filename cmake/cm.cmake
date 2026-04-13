message("Cmake Template. Language: C, Version: 4")

include("${CMAKE_CURRENT_LIST_DIR}/config.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/impl.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/target.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/tests.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/utils.cmake")

macro(cm_begin)
	cm_check_build_type()
	cm_c_set()
endmacro()


macro(cm_end)
	cm_output_dirs_set()
	cm_impl_bridge()
	cm_clangd_commands_json()
endmacro()

