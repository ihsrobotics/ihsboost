if (${build_controllers})
    message("building controllers")

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers.cpp)
    set(DEPS util)

    add_module(controllers "${SOURCES}")
    add_ihsboost_deps(controllers "${DEPS}")
    
    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/controllers)
    install(FILES ${install_file_dir}/controllers.hpp DESTINATION ${local_install_path}/controllers)
endif()