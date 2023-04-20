/**
 * @file all.hpp
 * @author Eliot Hall
 * @brief master header that includes all other ihsboost headers
 * @version 0.1
 * @date 2023-02-24
 *
 * @copyright Copyright (c) 2023
 *
 */
/**
 * @example message_example.cpp
 * @example PosixQreceiver.cpp
 * @example PosixQsender.cpp
 * @example bluetooth_client.cpp
 * @example bluetooth_server.cpp
 *
 * @example movement_example.cpp
 * @example encoder_example.cpp
 * @example pid_example.cpp
 * @example gyro_example.cpp
 * @example gyro_client_example.cpp
 *
 * @example threading_example.cpp
 *
 * @example create_extra_example.cpp
 *
 * @example json_loader.cpp
 * @example json_loader.hpp
 * @example bot-config.json
 * @example other-config.json
 * @example configure_ihsboost.cpp
 * @example ihsboost_config.json
 *
 * @example listener.cpp
 * @example listener.py
 * @example sender.py
 * @example movement_bindings.py
 * @example back_and_forth.py
 * @example back_and_forth.cpp
 * @example encoder_example.py
 */

#include "modules.hpp"

// communication
#ifdef build_communicate
#include "ihsboost/communicate/communicate.hpp"
#endif

// controllers
#ifdef build_controllers
#include "ihsboost/controllers/controllers.hpp"
#endif

// create_extra
#ifdef build_create_extra
#include "ihsboost/create_extra/create_extra.hpp"
#endif

// filters
#ifdef build_filters
#include "ihsboost/filters/filters.hpp"
#endif

// movement
#ifdef build_movement
#include "ihsboost/movement/movement.hpp"
#endif
// servos
#ifdef build_servos
#include "ihsboost/servos/servos.hpp"
#endif

// threading
#ifdef build_threading
#include "ihsboost/threading/threading.hpp"
#endif

// util
#ifdef build_util
#include "ihsboost/util/util.hpp"
#endif
