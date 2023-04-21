if(${build_bindings})
    set(python_version "3.9" CACHE STRING "Python version to use")

    # set install path
    set(python_install_path ${local_install_path}/../../lib/python${python_version}/dist-packages)
    message("using python module install path of " ${python_install_path})

    # configure
    if (python_version EQUAL 3.9)
        set(python_nodot_version 39)
        message("using python 3.9")
    elseif (python_version EQUAL 3.10)
        message("using python 3.10")
        set(python_nodot_version 310)
    else()
        message("Invalid python version!")
    endif()
    find_library(python_libs boost_python${python_nodot_version} REQUIRED)
    include_directories(${CMAKE_SYSROOT}/usr/include/python${python_version})

    # include all the correct directories
    include_directories(${CMAKE_SOURCE_DIR}/modules/include/)
    include_directories(${CMAKE_SOURCE_DIR}/modules/communicate/include/ihsboost/communicate/)
    include_directories(${CMAKE_SOURCE_DIR}/modules/controllers/include/ihsboost/controllers)
    include_directories(${CMAKE_SOURCE_DIR}/modules/create_extra/include/ihsboost/create_extra)
    include_directories(${CMAKE_SOURCE_DIR}/modules/servos/include/ihsboost/servos)
    include_directories(${CMAKE_SOURCE_DIR}/modules/threading/include/ihsboost/threading)
    include_directories(${CMAKE_SOURCE_DIR}/modules/util/include/ihsboost/util)
    include_directories(${CMAKE_SOURCE_DIR}/modules/movement/include/ihsboost/movement)

    # make it a python-able .so file by with the correct suffix/prefix
    set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/communicate_bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/create_extra_bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/movement_bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/servos_bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/util_bindings.cpp)
    add_library(ihs_bindings SHARED ${SOURCES})
    set_target_properties(ihs_bindings PROPERTIES PREFIX "")
    set_target_properties(ihs_bindings PROPERTIES SUFFIX .cpython-${python_nodot_version}-${CMAKE_SYSTEM_PROCESSOR}-linux-gnu.so)
    target_compile_definitions(ihs_bindings PUBLIC BOOST_BIND_GLOBAL_PLACEHOLDERS BOOST_ALLOW_DEPRECATED_HEADERS)  # ignore warnings

    # link to dependency libraries
    add_dependencies(ihs_bindings ihsboost)
    target_link_libraries(ihs_bindings ${python_libs} ihsboost)

    # install
    install(TARGETS ihs_bindings DESTINATION ${python_install_path})
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/ihs_bindings.pyi DESTINATION ${python_install_path})
endif()