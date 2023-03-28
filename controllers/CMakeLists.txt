message("building " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# include dependencies
include_directories(${CMAKE_SOURCE_DIR}/util/include)

#library
add_library(ihs_controllers OBJECT ${SOURCES})

# library dependencies
add_dependencies(ihs_controllers ihs_util)

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/controllers.hpp DESTINATION ${local_install_path})

# testing
if (${build_tests})
    add_executable(accelerator_test ${CMAKE_CURRENT_SOURCE_DIR}/test/accelerator_test.cpp)
    add_dependencies(accelerator_test ihsboost)
    target_link_libraries(accelerator_test ihsboost)
    target_include_directories(accelerator_test PUBLIC ${CMAKE_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR}/include)

    add_test(NAME accel_test COMMAND accelerator_test)
    set_tests_properties(accel_test PROPERTIES TIMEOUT 5)
endif()