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
 * @example message_buf_test.cpp
 * @example message_test.cpp
 * @example PosixQreceiver.cpp
 * @example PosixQsender.cpp
 *
 * @example movement_test.cpp
 * @example encoder_test.cpp
 * @example pid_test.cpp
 * @example gyro_test.cpp
 *
 * @example threading_test.cpp
 *
 * @example create_extra_test.cpp
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
 */

// controllers
#include "controllers.hpp"

//  movement
#include "movement.hpp"

// threading
#include "threading.hpp"

// util
#include "util.hpp"

// servos
#include "servos.hpp"

// communication
#include "communicate.hpp"

// create_extra
#include "create_extra.hpp"