message("building " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/speed.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/json_config.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/config.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# library
add_library(ihs_util OBJECT ${SOURCES})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/cliff.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/speed.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/timer.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_config.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/small_bot_config.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/json_config.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/roomba_config.json 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/small_bot_config.json ${CMAKE_CURRENT_SOURCE_DIR}/include/util.hpp
    DESTINATION ${local_install_path})

if (${build_tests})
    add_executable(config_test ${CMAKE_CURRENT_SOURCE_DIR}/test/config_test.cpp)
    add_dependencies(config_test ihsboost)
    target_link_libraries(config_test ihsboost)
    target_include_directories(config_test PUBLIC ${CMAKE_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR}/include)

    add_test(NAME configs COMMAND config_test WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/util)
endif()