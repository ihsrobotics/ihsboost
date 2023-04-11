if (${build_servos})
    message("building servos")

    # requirements
    find_library(kipr_libs kipr REQUIRED)

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/servos.cpp)
    set(DEPS controllers util)
    set(EXTERNAL_DEPS kipr)

    add_module(servos "${SOURCES}")
    add_ihsboost_deps(servos "${DEPS}")
    add_external_deps(servos "${EXTERNAL_DEPS}")

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/servos)
    install(FILES ${install_file_dir}/servos.hpp DESTINATION ${local_install_path}/servos)
endif()