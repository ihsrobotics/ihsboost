if (${build_filters})
    message("building filters")

    # header only library
    add_module(filters ${CMAKE_SOURCE_DIR}/empty.cpp)

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/filters/)
    install(FILES ${install_file_dir}/filters.hpp
        ${install_file_dir}/filter.hpp
        ${install_file_dir}/running_average.hpp
        DESTINATION ${local_install_path}/filters)
endif()