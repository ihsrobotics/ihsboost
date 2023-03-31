# global includes
include(CTest)

function(add_ihsboost_test_executable _name _file _module_depends)
    add_executable(${_name} ${_file})
    target_include_directories(${_name} PUBLIC ${CMAKE_SOURCE_DIR}/tests/include ${CMAKE_SOURCE_DIR}/modules/${_module_depends}/include)
    add_dependencies(${_name} ihsboost)
    target_link_libraries(${_name} ihsboost)
endfunction()

set(DIRECTORIES bindings communicate controllers threading util)

subdirs(${DIRECTORIES})
