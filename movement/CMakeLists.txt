message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/accelerate.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/line_follow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imu_movement.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/roomba_movement.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# include dependencies
include_directories(${CMAKE_SOURCE_DIR}/util/include)
include_directories(${CMAKE_SOURCE_DIR}/controllers/include)
include_directories(${CMAKE_SOURCE_DIR}/threading/include)

# library
add_library(ihs_movement OBJECT ${SOURCES})

# library dependencies
add_dependencies(ihs_movement ihs_threading ihs_util ihs_controllers)

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/accelerate.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/imu_movement.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/line_follow.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_movement.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/movement.hpp
    DESTINATION ${local_install_path})