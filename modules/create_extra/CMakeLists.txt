if (${build_create_extra})
    message("building create extra")

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/create_extra.cpp)
    set(EXTERNAL_DEPS kipr)

    add_module(create_extra "${SOURCES}")
    add_external_deps(create_extra "${EXTERNAL_DEPS}")

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/create_extra)
    install(FILES ${install_file_dir}/create_extra.hpp DESTINATION ${local_install_path}/create_extra)
endif()