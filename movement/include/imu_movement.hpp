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
#ifndef GYRO_MOVEMENT_HPP
#define GYRO_MOVEMENT_HPP

#include "config.hpp"
#include "speed.hpp"
#include <functional>

/**
 * @brief Drive the create straight using the gyroscope
 *
 * @param from_speed speed to start at
 * @param to_speed speed to cap at
 * @param stop_function a function that returns true when it is time to stop driving forward
 * @param correction_proportion how much the correction is by; defaults to .90,
 * should be between (0, 1)
 * @param accel_per_sec how fast the create will accelerate by, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_drive_straight(int from_speed, int to_speed, std::function<bool()> stop_function, double correction_proportion = get_config().getDouble("gyro_correction_proportion"), double accel_per_sec = get_config().getDouble("gyro_linear_accel"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

/**
 * @brief Turn the create a certain number of degrees using the gyroscope
 *
 * @param from_speed the speed to start at
 * @param to_speed the speed to end at
 * @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_turn_degrees(Speed from_speed, Speed to_speed, int degrees, double accel_per_sec = get_config().getDouble("gyro_linear_accel"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

/**
 * @brief Turns the create, starting at rest and ending at rest, and turning at max at max_speed
 *
 * @param max_speed the maximum speed to turn at, use a positive value
 * @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
 * @param min_speed the minimum speed to turn at, use a positive value, defaults to 11
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_turn_degrees_v2(int max_speed, int degrees, int min_speed = get_config().getInt("gyro_turn_v2_min_speed"), double accel_per_sec = get_config().getDouble("gyro_linear_accel"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

#endif
/**@}*/