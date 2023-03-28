cmake_minimum_required(VERSION 3.0)
project(ihs_boost VERSION 1.3.1)

set(LIBRARY_DIRS communicate controllers create_extra
                movement servos threading util
)

# configure config.hpp
option(roomba "Whether to build for the roomba or not" ON)
if (${roomba})
    set(robot_config_name "roomba_config.json")
    set(robot_config_hpp "roomba_config.hpp")
else()
    set(robot_config_name "small_bot_config.json")
    set(robot_config_hpp "small_bot_config.hpp")
endif()
configure_file(${CMAKE_SOURCE_DIR}/util/include/config.hpp.in ${CMAKE_SOURCE_DIR}/util/include/config.hpp)

# main .so file
option(build_library "build_library" ON)
if (${build_library})
    add_compile_options("-std=c++11")
    add_compile_options("-fPIC")
    add_compile_options("-Wall")
    add_compile_options("-Wconversion")

    find_library(json_libs jsoncpp REQUIRED)
    
    # install path configuration, defaults to /usr/local/
    message("installing to " ${CMAKE_INSTALL_PREFIX})
    define_property(GLOBAL PROPERTY install_path BRIEF_DOCS "where to install" FULL_DOCS "where to install files")
    set_property(GLOBAL PROPERTY install_path ${CMAKE_INSTALL_PREFIX}/include/ihsboost)
    get_property(local_install_path GLOBAL PROPERTY install_path)
    message("include install_path has been set to " ${local_install_path})

    # add subdirectories
    foreach(subdir IN LISTS LIBRARY_DIRS)
        add_subdirectory(${CMAKE_SOURCE_DIR}/${subdir})
    endforeach()

    # link
    add_library(ihsboost SHARED empty.cpp)
    target_link_libraries(ihsboost PUBLIC kipr pthread rt ${json_libs})
    foreach(subdir IN LISTS LIBRARY_DIRS)
        target_link_libraries(ihsboost PUBLIC ihs_${subdir})
    endforeach()

    # install
    install(TARGETS ihsboost DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
    install(FILES all.hpp DESTINATION ${local_install_path})
endif()

# python bindings
option(build_python "build_python" ON)
if (${build_python})
    add_subdirectory(bindings)
endif()

# doxygen/documentation
option(with_documentation "with_documentation" OFF)
if (${with_documentation})
    find_package(Doxygen REQUIRED)

    # include the extra files
    set(DOXYGEN_SOURCES ${LIBRARY_DIRS} ${CMAKE_SOURCE_DIR}/README.md ${CMAKE_SOURCE_DIR}/all.hpp ${CMAKE_SOURCE_DIR}/bindings/)
    set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR}/examples)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)

    # doxygen it
    doxygen_add_docs(doxygen ${DOXYGEN_SOURCES} ALL)
endif()