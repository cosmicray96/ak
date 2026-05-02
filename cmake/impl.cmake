set(cm_impl_targets "")
macro(cm_impl_bridge)

list(FIND cm_impls "x11" idx)
if(idx GREATER -1)
	set(target_name "x11_wrapper")
add_subdirectory("${cm_proj_root_dir}/projects/${target_name}")

    list(APPEND cm_impl_targets "${target_name}")
    list(SORT cm_impl_targets)
endif()

list(FIND cm_impls "opengl" idx)
if(idx GREATER -1)
	set(target_name "opengl_wrapper")
add_subdirectory("${cm_proj_root_dir}/projects/${target_name}")

    list(APPEND cm_impl_targets "${target_name}")
    list(SORT cm_impl_targets)
endif()

endmacro()
