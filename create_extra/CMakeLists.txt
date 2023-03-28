message("building " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/create_extra.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#library
add_library(ihs_create_extra OBJECT ${SOURCES})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/create_extra.hpp DESTINATION ${local_install_path})