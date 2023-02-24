message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# add a dummy library
add_library(ihs_threading OBJECT ${CMAKE_SOURCE_DIR}/empty.cpp)

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/threading.hpp DESTINATION ${local_install_path})