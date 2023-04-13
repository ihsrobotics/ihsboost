/**
 * @file imu_movement.hpp
 * @author Eliot Hall
 * @brief IMU (gyro/accel) based movement functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef IHSBOOST_GYRO_MOVEMENT_HPP
#define IHSBOOST_GYRO_MOVEMENT_HPP

#include "ihsboost/threading/accumulator.hpp"
#include "ihsboost/util/config.hpp"
#include "ihsboost/util/speed.hpp"
#include <functional>

/**
 * @brief Drive straight using the gyroscope until told to stop
 *
 * @param speed the speed to drive at, can be positive or negative
 * @param stop_function the stop condition; it should return true when it is time to stop
 * @param stop whether or not to stop the motors after finishing driving
 * @param correction_proportion how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
 * @param updates_per_sec how many updates to do per second
 */
void gyro_drive_straight(int speed, std::function<bool()> stop_function, bool stop = get_config().getBool("gyro_stop"), double correction_proportion = get_config().getDouble("gyro_correction_proportion"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

/**
 * @brief Turn the create a certain number of degrees using the gyroscope
 *
 * @param turn_speed the speed to turn at
 * @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_turn_degrees(Speed turn_speed, double degrees, int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

/**
 * @brief Turns the create, starting at rest and ending at rest, and turning at max at max_speed
 *
 * @param max_speed the maximum speed to turn at, use a positive value
 * @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
 * @param min_speed the minimum speed to turn at, use a positive value, defaults to 11
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_turn_degrees(int max_speed, double degrees, int min_speed = get_config().getInt("gyro_min_speed"), double accel_per_sec = get_config().getDouble("gyro_accel_per_sec"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

#endif
/**@}*/