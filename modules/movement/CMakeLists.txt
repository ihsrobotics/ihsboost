if (${build_movement})
    message("building movement")

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/accelerate.cpp 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/imu_movement.cpp 
                ${CMAKE_CURRENT_SOURCE_DIR}/src/line_follow.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/maneuvers.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/roomba_movement.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/singletons.cpp
                )
    set(DEPS util controllers threading)
    set(EXTERNAL_DEPS kipr)

    add_module(movement "${SOURCES}")
    add_ihsboost_deps(movement "${DEPS}")
    add_external_deps(movement "${EXTERNAL_DEPS}")

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/movement/)
    install(FILES ${install_file_dir}/accelerate.hpp 
        ${install_file_dir}/imu_movement.hpp 
        ${install_file_dir}/line_follow.hpp
        ${install_file_dir}/maneuvers.hpp 
        ${install_file_dir}/movement_constants.hpp
        ${install_file_dir}/movement.hpp
        ${install_file_dir}/roomba_movement.hpp
        ${install_file_dir}/singletons.hpp
        DESTINATION ${local_install_path}/movement)
endif()