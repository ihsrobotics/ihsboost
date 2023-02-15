cmake_minimum_required(VERSION 3.0)
project(ihs_boost)

option("with_documentation" OFF)
if (with_documentation)
    find_package(Doxygen REQUIRED)

    doxygen_add_docs(doxygen ${CMAKE_SOURCE_DIR} ALL)
endif()

option("build_library" ON)
if(build_library)
    add_compile_options("-std=c++11")  # if 17 is not available, 11 will do for everything except for threading test
    add_compile_options("-fPIC")

    include_directories(/usr/local/include)
    message("installing to " ${CMAKE_INSTALL_PREFIX})
    define_property(GLOBAL PROPERTY install_path BRIEF_DOCS "where to install" FULL_DOCS "where to install files")
    set_property(GLOBAL PROPERTY install_path ${CMAKE_INSTALL_PREFIX}/include/ihsboost/)
    get_property(local_install_path GLOBAL PROPERTY install_path)
    define_property(GLOBAL PROPERTY IHS_SOURCES BRIEF_DOCS "the sources" FULL_DOCS "the full sources")
    message("include install_path has been set to " ${local_install_path})

    add_subdirectory(util)
    add_subdirectory(movement)
    add_subdirectory(controllers)
    add_subdirectory(threading)
    add_subdirectory(servos)
    add_subdirectory(communicate)
    add_subdirectory(create_extra)

    include_directories(util/include)
    include_directories(movement/include)
    include_directories(controllers/include)
    include_directories(threading/include)
    include_directories(servos/include)
    include_directories(communicate/include)

    get_property(local_ihs_sources GLOBAL PROPERTY IHS_SOURCES)
    message("Sources are" ${local_ihs_sources})
    add_library(ihsboost SHARED ${local_ihs_sources})
    target_link_libraries(ihsboost PUBLIC kipr pthread rt)

    # install
    install(TARGETS ihsboost DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
    install(FILES all.hpp DESTINATION ${local_install_path})
endif()