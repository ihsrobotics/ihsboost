# testing
if ((${build_tests} AND ${build_controllers}))
    add_ihsboost_test_executable(accelerator_test ${CMAKE_CURRENT_SOURCE_DIR}/test/accelerator_test.cpp controllers)

    add_test(NAME accel_test COMMAND accelerator_test)
    set_tests_properties(accel_test PROPERTIES TIMEOUT 5)
endif()