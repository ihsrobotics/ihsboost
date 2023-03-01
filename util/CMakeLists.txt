message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# configure config.hpp
option(robot "Whether to build for the roomba or not" ON)
if (${robot})
    set(robot_config_name "roomba_config.json")
    set(robot_config_hpp "roomba_config.hpp")
else()
    set(robot_config_name "small_bot_config.json")
    set(robot_config_hpp "small_bot_config.hpp")
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp)

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/speed.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/accumulator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/json_config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# dependencies
include_directories(${CMAKE_SOURCE_DIR}/threading/include/)

# library
add_library(ihs_util OBJECT ${SOURCES})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/cliff.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/speed.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/timer.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/accumulator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_config.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/small_bot_config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/json_config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_config.json ${CMAKE_CURRENT_SOURCE_DIR}/include/small_bot_config.json
    DESTINATION ${local_install_path})