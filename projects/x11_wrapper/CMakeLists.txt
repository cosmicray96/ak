find_package(X11 REQUIRED)

add_library(x11_wrapper INTERFACE)
target_include_directories(x11_wrapper INTERFACE ${X11_INCLUDE_DIR})
target_link_libraries(x11_wrapper INTERFACE ${X11_LIBRARIES})
