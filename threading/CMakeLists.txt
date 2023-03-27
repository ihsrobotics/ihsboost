message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# set sources
get_property(local_ihs_sources GLOBAL PROPERTY IHS_SOURCES)
list(APPEND local_ihs_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/threadable.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/background_task.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/accumulator.cpp)
set_property(GLOBAL PROPERTY IHS_SOURCES ${local_ihs_sources})


# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/threading.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/threadable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/accumulator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/background_task.hpp
        DESTINATION ${local_install_path})