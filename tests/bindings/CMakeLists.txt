# testing
if((${build_tests} AND ${build_bindings}))
    message("building bindings tests")
    add_ihsboost_test_executable(back_and_forth ${CMAKE_CURRENT_SOURCE_DIR}/test/back_and_forth_test.cpp communicate)
    
    add_test(NAME posix_binding_test COMMAND back_and_forth "PosixQCommunicator" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set_tests_properties(posix_binding_test PROPERTIES TIMEOUT 5)
    add_test(NAME socket_binding_test COMMAND back_and_forth "SocketCommunicator"  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set_tests_properties(socket_binding_test PROPERTIES TIMEOUT 5)
    add_test(NAME sysv_binding_test COMMAND back_and_forth "SysVCommunicator"  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
    set_tests_properties(sysv_binding_test PROPERTIES TIMEOUT 5)
endif()