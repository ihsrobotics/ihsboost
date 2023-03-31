if (${build_communicate})
    message("building communicate")

    # requirements
    find_library(bluetooth_libs bluetooth REQUIRED)

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/communicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/message_buf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/posixqcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/sysvcommunicator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/socketcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/communication_exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shmcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/btcommunicator.cpp)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    #library
    add_library(ihs_communicate OBJECT ${SOURCES})
    target_link_libraries(ihs_communicate PUBLIC ${bluetooth_libs} rt)

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/communicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/message_buf.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/posixqcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/sysvcommunicator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/socketcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/communication_exception.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shmcommunicator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/communicate.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/btcommunicator.hpp
        DESTINATION ${local_install_path})
endif()