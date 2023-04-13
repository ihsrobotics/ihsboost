/**
 * @file accelerate.hpp
 * @author Eliot Hall
 * @brief Acceleration functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef IHSBOOST_ACCELERATE_HPP
#define IHSBOOST_ACCELERATE_HPP

#include "ihsboost/util/config.hpp"
#include "ihsboost/util/speed.hpp"

/**
 * @brief Accelerate both wheels at the same linear rate
 *
 * @param from_speed the starting speed
 * @param to_speed the ending speed
 * @param accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_forward_linear(int from_speed, int to_speed, double accel_per_sec = get_config().getDouble("linear_accel"), int updates_per_sec = get_config().getInt("accelerate_updates_per_sec"));

/**
 * @brief Accelerate both wheels at the same sinusoidal rate
 *
 * @param from_speed the starting speed
 * @param to_speed the ending speed
 * @param avg_accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_forward_sin(int from_speed, int to_speed, double avg_accel_per_sec = get_config().getDouble("sinusoidal_accel"), int updates_per_sec = get_config().getInt("accelerate_updates_per_sec"));

/**
 * @brief Accelerate both wheels at potentially different linear rates
 *
 * @param from_speed the starting speed, in the form of {left_speed, right_speed}
 * @param to_speed the ending speed, in the form of {left_speed, right_speed}
 * @param accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_linear(Speed from_speed, Speed to_speed, double accel_per_sec = get_config().getDouble("linear_accel"), int updates_per_sec = get_config().getInt("accelerate_updates_per_sec"));

/**
 * @brief Accelerate both wheels at potentially different sinusoidal rates
 *
 * @param from_speed the starting speed, in the form of {left_speed, right_speed}
 * @param to_speed the ending speed, in the form of {left_speed, right_speed}
 * @param accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_sinusoidal(Speed from_speed, Speed to_speed, double accel_per_sec = get_config().getDouble("sinusoidal_accel"), int updates_per_sec = get_config().getInt("accelerate_updates_per_sec"));

#endif
/**@}*/