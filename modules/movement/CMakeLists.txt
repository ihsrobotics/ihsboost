if (${build_movement})
    message("building movement")

    # requirements
    find_library(kipr_libs kipr REQUIRED)

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/accelerate.cpp 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/imu_movement.cpp 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/line_follow.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/maneuvers.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/roomba_movement.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/singletons.cpp
                )
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    # include dependencies
    include_directories(${CMAKE_SOURCE_DIR}/modules/util/include)
    include_directories(${CMAKE_SOURCE_DIR}/modules/controllers/include)
    include_directories(${CMAKE_SOURCE_DIR}/modules/threading/include)

    # library
    add_library(ihs_movement OBJECT ${SOURCES})
    target_link_libraries(ihs_movement PUBLIC ${kipr_libs})

    # library dependencies
    add_dependencies(ihs_movement ihs_threading ihs_util ihs_controllers)

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/accelerate.hpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/imu_movement.hpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/line_follow.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/maneuvers.hpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/movement_constants.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/movement.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_movement.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/singletons.hpp
        DESTINATION ${local_install_path})
endif()