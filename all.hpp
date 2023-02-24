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
 *
 * @example movement_test.cpp
 * @example encoder_test.cpp
 * @example pid_test.cpp
 *
 * @example threading_test.cpp
 *
 * @example create_extra_test.cpp
 *
 * @example json_loader.cpp
 * @example json_loader.hpp
 * @example bot-config.json
 * @example other-config.json
 *
 * @example listener.cpp
 * @example listener.py
 * @example sender.py
 */

// controllers
#include "controllers.hpp"

//  movement
#include "accelerate.hpp"
#include "imu_movement.hpp"
#include "line_follow.hpp"
#include "roomba_movement.hpp"

// threading
#include "threading.hpp"

// util
#include "cliff.hpp"
#include "speed.hpp"
#include "timer.hpp"
#include "json_config.hpp"

// servos
#include "servos.hpp"

// communication
#include "communicator.hpp"
#include "posixqcommunicator.hpp"
#include "socketcommunicator.hpp"
#include "sysvcommunicator.hpp"
#include "shmcommunicator.hpp"

// create_extra
#include "create_extra.hpp"