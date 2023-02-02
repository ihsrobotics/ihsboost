message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
get_property(local_ihs_sources GLOBAL PROPERTY IHS_SOURCES)
list(APPEND local_ihs_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/servos.cpp)
set_property(GLOBAL PROPERTY IHS_SOURCES ${local_ihs_sources})
# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/servos.hpp DESTINATION ${local_install_path})