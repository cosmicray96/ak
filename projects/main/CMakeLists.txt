set(target_name "main")
set(targ_path "${cm_proj_root_dir}/projects/${target_name}")

cm_target_get_files_pub(${target_name} ${targ_path} files_pub)
cm_target_get_files_priv(${target_name} ${targ_path} files_priv)

cm_target_get_includes_pub(${target_name} ${targ_path} inc_pub)
cm_target_get_includes_priv(${target_name} ${targ_path} inc_priv)

cm_target_get_defines_pub(${target_name} ${targ_path} defs_pub)
cm_target_get_defines_priv(${target_name} ${targ_path} defs_priv)


add_executable(${target_name}
)
target_sources(${target_name}
	PUBLIC
	"${files_pub}"
	PRIVATE
	"${files_priv}"
)
target_include_directories(${target_name}
	PUBLIC
	"${inc_pub}"
	PRIVATE 
	"${inc_priv}"
)
target_compile_definitions(${target_name}
	PUBLIC
	"${defs_pub}"
	PRIVATE
	"${defs_priv}"
)


target_link_libraries(${target_name} PRIVATE "ak")
