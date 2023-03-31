# ========== Options Section ==========
# library option
option(build_library "build_library" ON)

# robot option
option(roomba "Whether to build for the roomba or not" ON)

# module options
option(build_bindings "whether or not to build python bindings" ON)
option(build_communicate "whether or not to build communicate classes" ON)
option(build_controllers "whether or not to build controller classes" ON)
option(build_create_extra "whether or not to build extra create functionality" ON)
option(build_movement "whether or not to build movement functionality" ON)
option(build_servos "whether or not to build servo functionality" ON)
option(build_threading "whether or not to build threading functionality" ON)
option(build_util "whether or not to build util functionality" ON)

# ========== Configure Section ==========
# configure modules
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/modules.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/modules.hpp @ONLY)

# configure config.hpp
if (${roomba})
set(robot_config_name "roomba_config.json")
    set(robot_config_hpp "roomba_config.hpp")
else()
    set(robot_config_name "small_bot_config.json")
    set(robot_config_hpp "small_bot_config.hpp")
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/util/include/config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/util/include/config.hpp)

# ========== Library Section ==========
if (${build_library})
    # set directories
    set(DIRECTORIES bindings communicate controllers create_extra movement servos threading util)

    # add compile options
    add_compile_options("-std=c++11")
    add_compile_options("-fPIC")
    add_compile_options("-Wall")
    add_compile_options("-Wconversion")

    # install path configuration, defaults to /usr/local/
    message("installing to " ${CMAKE_INSTALL_PREFIX})
    define_property(GLOBAL PROPERTY install_path BRIEF_DOCS "where to install" FULL_DOCS "where to install files")
    set_property(GLOBAL PROPERTY install_path ${CMAKE_INSTALL_PREFIX}/include/ihsboost)
    get_property(local_install_path GLOBAL PROPERTY install_path)
    message("include install_path has been set to " ${local_install_path})

    # add subdirectories
    subdirs(${DIRECTORIES})

    # link library
    add_library(ihsboost SHARED ${CMAKE_SOURCE_DIR}/empty.cpp)
    foreach(subdir IN LISTS DIRECTORIES)
        # don't link bindings to ihsboost
        if ((NOT ${subdir} STREQUAL "bindings" AND ${build_${subdir}}))
            target_link_libraries(ihsboost PUBLIC ihs_${subdir})
        endif()
    endforeach()

    # install
    install(TARGETS ihsboost DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
    install(FILES include/all.hpp include/modules.hpp DESTINATION ${local_install_path})
endif()