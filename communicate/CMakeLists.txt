message("building " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/communicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/message_buf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/posixqcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/sysvcommunicator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/socketcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/communication_exception.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shmcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/btcommunicator.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#library
add_library(ihs_communicate OBJECT ${SOURCES})

# install
get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/communicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/message_buf.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/posixqcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/sysvcommunicator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/socketcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/communication_exception.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/shmcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/communicate.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/btcommunicator.hpp
    DESTINATION ${local_install_path})

# testing
if (${build_tests})
    # build test executables
    # regular communicator tests
    add_executable(com_test ${CMAKE_CURRENT_SOURCE_DIR}/test/communicate_test.cpp)
    target_include_directories(com_test PUBLIC ${CMAKE_SOURCE_DIR}/test/ ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_dependencies(com_test ihsboost)
    target_link_libraries(com_test ihsboost)
    # shm tests
    add_executable(shm_test ${CMAKE_CURRENT_SOURCE_DIR}/test/shm_test.cpp)
    target_include_directories(shm_test PUBLIC ${CMAKE_SOURCE_DIR}/test/ ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_dependencies(shm_test ihsboost)
    target_link_libraries(shm_test ihsboost)
    # message buf test
    add_executable(messagebuf ${CMAKE_CURRENT_SOURCE_DIR}/test/message_buf_test.cpp)
    target_include_directories(messagebuf PUBLIC ${CMAKE_SOURCE_DIR}/test/ ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_dependencies(messagebuf ihsboost)
    target_link_libraries(messagebuf ihsboost)

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