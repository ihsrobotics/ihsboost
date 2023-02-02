message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
get_property(local_ihs_sources GLOBAL PROPERTY IHS_SOURCES)
list(APPEND local_ihs_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/accelerate.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/line_follow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imu_movement.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/roomba_turns.cpp)
set_property(GLOBAL PROPERTY IHS_SOURCES ${local_ihs_sources})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/accelerate.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/imu_movement.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/line_follow.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_turns.hpp
    DESTINATION ${local_install_path})