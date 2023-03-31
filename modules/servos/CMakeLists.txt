if (${build_servos})
    message("building servos")

    # requirements
    find_library(kipr_libs kipr REQUIRED)

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/servos.cpp)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    # include dependencies
    include_directories(${CMAKE_SOURCE_DIR}/modules/controllers/include)
    include_directories(${CMAKE_SOURCE_DIR}/modules/util/include)

    # library
    add_library(ihs_servos OBJECT ${SOURCES})
    target_link_libraries(ihs_servos PUBLIC ${kipr_libs})

    # library dependencies
    add_dependencies(ihs_servos ihs_controllers)

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/servos.hpp DESTINATION ${local_install_path})
endif()