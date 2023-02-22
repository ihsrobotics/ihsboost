option(python_verion "What python version to use" 3.9)

# output messages
message("python version is " ${python_version})
set(python_install_path /usr/local/lib/python${python_version}/dist-packages)
message("python module install path is " ${python_install_path})

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
find_library(python_libs boost_python${python_nodot_version})
include_directories(/usr/include/python${python_version})

# include all the correct directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../communicate/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../controllers/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../create_extra/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../servos/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../threading/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../util/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../movement/include)

# make it a python-able .so file by with the correct suffix/prefix
set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/bindings.cpp)
add_library(ihs_bindings SHARED ${SOURCES})
set_target_properties(ihs_bindings PROPERTIES PREFIX "")
set_target_properties(ihs_bindings PROPERTIES SUFFIX .cpython-${python_nodot_version}-${CMAKE_HOST_SYSTEM_PROCESSOR}-linux-gnu.so)

# link to dependency libraries
add_dependencies(ihs_bindings ihsboost)
target_link_libraries(ihs_bindings ${python_libs} pthread util dl ihsboost)

# install
install(TARGETS ihs_bindings DESTINATION ${python_install_path})