cmake_minimum_required(VERSION 3.0)
project(ihs_boost VERSION 1.5.0)

# options
option(build_tests "build_tests" OFF)
option(with_documentation "with_documentation" OFF)

# add modules
add_subdirectory(modules)

# tests for ctest
if (${build_tests})
    include(CTest)
    add_subdirectory(tests)
endif()

# doxygen/documentation
if (${with_documentation})
    find_package(Doxygen REQUIRED)

    # include the extra files
    set(DOXYGEN_SOURCES ${CMAKE_SOURCE_DIR}/README.md ${CMAKE_SOURCE_DIR}/modules/)
    set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR}/examples)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)

    # doxygen it
    doxygen_add_docs(doxygen ${DOXYGEN_SOURCES} ALL)
endif()