set(python_version "3.9" CACHE STRING "Python version to use")

# set install path
set(python_install_path /usr/local/lib/python${python_version}/dist-packages)
message("using python module install path of " ${python_install_path})

# configure
if (python_version EQUAL 3.9)
    set(python_nodot_version 39)
    message("using python 3.9")
elseif (python_version EQUAL 3.10)
message("using python 3.10")
    set(python_nodot_version 310)
else()
    message("Invalid python version!")
endif()
find_library(python_libs boost_python${python_nodot_version} REQUIRED)
include_directories(/usr/include/python${python_version})

# include all the correct directories
include_directories(${CMAKE_SOURCE_DIR}/..)
include_directories(${CMAKE_SOURCE_DIR}/communicate/include)
include_directories(${CMAKE_SOURCE_DIR}/controllers/include)
include_directories(${CMAKE_SOURCE_DIR}/create_extra/include)
include_directories(${CMAKE_SOURCE_DIR}/servos/include)
include_directories(${CMAKE_SOURCE_DIR}/threading/include)
include_directories(${CMAKE_SOURCE_DIR}/util/include)
include_directories(${CMAKE_SOURCE_DIR}/movement/include)

# make it a python-able .so file by with the correct suffix/prefix
set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/communicate_bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/create_extra_bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/movement_bindings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/servos_bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/util_bindings.cpp)
add_library(ihs_bindings SHARED ${SOURCES})
set_target_properties(ihs_bindings PROPERTIES PREFIX "")
set_target_properties(ihs_bindings PROPERTIES SUFFIX .cpython-${python_nodot_version}-${CMAKE_HOST_SYSTEM_PROCESSOR}-linux-gnu.so)

# link to dependency libraries
add_dependencies(ihs_bindings ihsboost)
target_link_libraries(ihs_bindings ${python_libs} pthread util dl ihsboost)

# install
install(TARGETS ihs_bindings DESTINATION ${python_install_path})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/ihs_bindings.pyi DESTINATION ${python_install_path})

# testing
if(${build_tests})
    add_executable(back_and_forth ${CMAKE_CURRENT_SOURCE_DIR}/test/back_and_forth_test.cpp)
    add_dependencies(back_and_forth ihsboost)
    target_link_libraries(back_and_forth ihsboost)
    target_include_directories(back_and_forth PUBLIC ${CMAKE_SOURCE_DIR}/test)
    
    add_test(NAME posix_binding_test COMMAND back_and_forth "PosixQCommunicator" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set_tests_properties(posix_binding_test PROPERTIES TIMEOUT 5)
    add_test(NAME socket_binding_test COMMAND back_and_forth "SocketCommunicator"  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set_tests_properties(socket_binding_test PROPERTIES TIMEOUT 5)
    add_test(NAME sysv_binding_test COMMAND back_and_forth "SysVCommunicator"  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set_tests_properties(sysv_binding_test PROPERTIES TIMEOUT 5)
endif()