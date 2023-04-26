# testing
if ((${build_tests} AND ${build_filters}))
    add_ihsboost_test_executable(ra_test ${CMAKE_CURRENT_SOURCE_DIR}/test/ratest.cpp filters)

    add_test(NAME running_average_test COMMAND ra_test)
    set_tests_properties(running_average_test PROPERTIES TIMEOUT 5)
endif()