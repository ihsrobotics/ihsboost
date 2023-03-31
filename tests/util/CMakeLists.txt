if ((${build_tests} AND ${build_util}))
    add_ihsboost_test_executable(config_test ${CMAKE_CURRENT_SOURCE_DIR}/test/config_test.cpp util)

    add_test(NAME configs COMMAND config_test WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/modules/util)
endif()