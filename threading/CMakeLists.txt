message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# set sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/background_task.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/accumulator.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add library
add_library(ihs_threading OBJECT ${SOURCES})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/threading.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/threadable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/accumulator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/background_task.hpp
        DESTINATION ${local_install_path})