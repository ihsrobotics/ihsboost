/**
 * @file servos.hpp
 * @author Eliot Hall
 * @brief Servo movement functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @defgroup servos_id Servos Functions
 * @{
 */
#ifndef IHSBOOST_SERVOS_HPP
#define IHSBOOST_SERVOS_HPP

#include "ihsboost/util/config.hpp"

/**
 * @brief Move a servo slowly to its goal position
 *
 * @param port the port where the servo is
 * @param position the goal position to move the servo to
 * @param speed how fast to move the servo, in ticks per second. Defaults to 200
 * @param updates_per_sec How many updates to do per second, defaults to 100
 */
void move_servo_slowly(int port, int position, int speed = get_config().getInt("servo_movement_speed"), int updates_per_sec = get_config().getInt("servo_updates_per_sec"));

#endif
/**@}*/