message("current source dir is " ${CMAKE_CURRENT_SOURCE_DIR})

# sources
get_property(local_ihs_sources GLOBAL PROPERTY IHS_SOURCES)
list(APPEND local_ihs_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/communicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/posixqcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/sysvcommunicator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/socketcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/communication_exception.cpp)
set_property(GLOBAL PROPERTY IHS_SOURCES ${local_ihs_sources})

get_property(local_install_path GLOBAL PROPERTY install_path)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/communicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/message.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/posixqcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/sysvcommunicator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/socketcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/communication_exception.hpp
    DESTINATION ${local_install_path})