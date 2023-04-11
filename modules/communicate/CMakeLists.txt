if (${build_communicate})
    message("building communicate")

    # sources
    set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/communicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/message_buf.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/posixqcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/sysvcommunicator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/socketcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/communication_exception.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shmcommunicator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/btcommunicator.cpp)
    set(EXTERNAL_DEPS bluetooth rt)

    add_module(communicate "${SOURCES}")
    add_external_deps(communicate "${EXTERNAL_DEPS}")

    # install
    get_property(local_install_path GLOBAL PROPERTY install_path)
    set(install_file_dir ${CMAKE_CURRENT_SOURCE_DIR}/include/ihsboost/communicate/)
    install(FILES ${install_file_dir}/communicator.hpp ${install_file_dir}/message_buf.hpp
        ${install_file_dir}/posixqcommunicator.hpp ${install_file_dir}/sysvcommunicator.hpp
        ${install_file_dir}/socketcommunicator.hpp ${install_file_dir}/communication_exception.hpp
        ${install_file_dir}/shmcommunicator.hpp ${install_file_dir}/communicate.hpp
        ${install_file_dir}/btcommunicator.hpp
        DESTINATION ${local_install_path}/communicate)
endif()