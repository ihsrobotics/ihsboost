message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})
message("no need to build library for threading")

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/threading.hpp DESTINATION ${local_install_path})