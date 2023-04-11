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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/util/include/ihsboost/util/config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/util/include/ihsboost/util/config.hpp)

# ========== Module adding functions ==========
function(add_module _name sources)
    add_library(${_name} OBJECT ${sources})
    target_include_directories(${_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/${_name}/)
endfunction()
function(add_ihsboost_deps _name ihsboost_dependencies)
    # add the ihsboost dependencies
    foreach(dependency IN LISTS ihsboost_dependencies)
        target_include_directories(${_name} PUBLIC ${CMAKE_SOURCE_DIR}/modules/${dependency}/include/)
    endforeach()
endfunction()
function(add_external_deps _name external_dependencies)
    # add the external dependencies
    foreach(external_dep IN LISTS external_dependencies)
        find_library(lib_${external_dep} ${external_dep} REQUIRED)
        target_link_libraries(${_name} PUBLIC ${lib_${external_dep}})
    endforeach()
endfunction()

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
            message("linking" ${subdir})
            target_link_libraries(ihsboost PUBLIC ${subdir})
        endif()
    endforeach()

    # install
    install(TARGETS ihsboost DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
    install(FILES include/all.hpp include/modules.hpp DESTINATION ${local_install_path})
endif()