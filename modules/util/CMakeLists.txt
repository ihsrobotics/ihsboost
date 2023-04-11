if (${build_util})
    message("building util")

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/speed.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/json_config.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp)
    set(EXTERNAL_DEPS jsoncpp)

    add_module(util "${SOURCES}")
    add_external_deps(util "${EXTERNAL_DEPS}")

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/util)
    install(FILES ${install_file_dir}/cliff.hpp ${install_file_dir}/speed.hpp 
        ${install_file_dir}/timer.hpp ${install_file_dir}/roomba_config.hpp 
        ${install_file_dir}/small_bot_config.hpp ${install_file_dir}/config.hpp 
        ${install_file_dir}/json_config.hpp ${install_file_dir}/roomba_config.json 
        ${install_file_dir}/small_bot_config.json ${install_file_dir}/util.hpp
        DESTINATION ${local_install_path}/util)
endif()