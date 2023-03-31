
# testing
if ((${build_tests} AND ${build_communicate}))
    message("building communicate tests")
    # build test executables
    # regular communicator tests
    add_ihsboost_test_executable(com_test ${CMAKE_CURRENT_SOURCE_DIR}/test/communicate_test.cpp communicate)
    # shm tests
    add_ihsboost_test_executable(shm_test ${CMAKE_CURRENT_SOURCE_DIR}/test/shm_test.cpp communicate)
    # message buf test
    add_ihsboost_test_executable(messagebuf ${CMAKE_CURRENT_SOURCE_DIR}/test/message_buf_test.cpp communicate)

    # add tests
    # add posix tests
    add_test(NAME posix_test COMMAND com_test "PosixQCommunicator")
    set_tests_properties(posix_test PROPERTIES TIMEOUT 10)
    # add socket test
    add_test(NAME socket_test COMMAND com_test "SocketCommunicator")
    set_tests_properties(socket_test PROPERTIES TIMEOUT 10)
    # add sysv test
    add_test(NAME sysv_test COMMAND com_test "SysVCommunicator")
    set_tests_properties(sysv_test PROPERTIES TIMEOUT 10)
    # add shm test
    add_test(NAME shared_memory_test COMMAND shm_test "sender")
    set_tests_properties(shared_memory_test PROPERTIES TIMEOUT 10)
    # add messagebuf test
    add_test(NAME messagebuf_test COMMAND messagebuf)
    set_tests_properties(messagebuf_test PROPERTIES TIMEOUT 5)
endif()