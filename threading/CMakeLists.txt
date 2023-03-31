message("building " ${CMAKE_CURRENT_SOURCE_DIR})

# set sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/threadable.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/background_task.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/accumulator.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add library
add_library(ihs_threading OBJECT ${SOURCES})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/threading.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/threadable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/accumulator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/background_task.hpp
        DESTINATION ${local_install_path})

if (${build_tests})
        add_executable(threader ${CMAKE_CURRENT_SOURCE_DIR}/test/thread_test.cpp)
        target_include_directories(threader PUBLIC ${CMAKE_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR}/include)
        add_dependencies(threader ihsboost)
        target_link_libraries(threader ihsboost)

        add_test(NAME thread_test COMMAND threader)
        set_tests_properties(thread_test PROPERTIES TIMEOUT 20)
endif()