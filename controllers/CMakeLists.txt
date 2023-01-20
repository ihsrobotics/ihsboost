message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/accelerator.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# include dependencies
include_directories(${CMAKE_SOURCE_DIR}/util/include)

#library
add_library(ihs_controllers OBJECT ${SOURCES})

# library dependencies
add_dependencies(ihs_controllers ihs_util)

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/accelerator.hpp DESTINATION ${local_install_path})