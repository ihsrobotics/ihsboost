if (${build_threading})
    message("building threading")

    # set sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/threadable.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/background_task.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/accumulator.cpp)
    set(EXTERNAL_DEPS pthread)

    add_module(threading "${SOURCES}")
    add_external_deps(threading "${EXTERNAL_DEPS}")

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/threading)
    install(FILES ${install_file_dir}/threading.hpp ${install_file_dir}/threadable.hpp
                  ${install_file_dir}/accumulator.hpp ${install_file_dir}/background_task.hpp
                  DESTINATION ${local_install_path}/threading)
endif()