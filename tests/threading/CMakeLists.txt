
if ((${build_tests} AND ${build_threading}))
    add_ihsboost_test_executable(threader ${CMAKE_CURRENT_SOURCE_DIR}/test/thread_test.cpp threading)

    add_test(NAME thread_test COMMAND threader)
    set_tests_properties(thread_test PROPERTIES TIMEOUT 20)
endif()