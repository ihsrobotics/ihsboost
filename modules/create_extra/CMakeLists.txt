if (${build_create_extra})
    message("building create extra")

    # requirements
    find_library(kipr_libs kipr REQUIRED)

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/create_extra.cpp)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    #library
    add_library(ihs_create_extra OBJECT ${SOURCES})
    target_link_libraries(ihs_create_extra PUBLIC ${kipr_libs})

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/create_extra.hpp DESTINATION ${local_install_path})
endif()