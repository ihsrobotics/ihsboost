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
#ifndef ACCELERATE_HPP
#define ACCELERATE_HPP

#include "speed.hpp"

#define LINEAR_ACCEL_PER_SEC 500       ///< default linear acceleration; should always be positive
#define SIN_ACCEL_AVG_PER_SEC 500      ///< default sinusoidal acceleration; should always be positive
#define ACCELERATE_UPDATES_PER_SEC 500 ///< default updates per second when accelerating

/**
 * @brief Accelerate both wheels at the same linear rate
 *
 * @param from_speed the starting speed
 * @param to_speed the ending speed
 * @param accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_forward_linear(int from_speed, int to_speed, double accel_per_sec = LINEAR_ACCEL_PER_SEC, int updates_per_sec = ACCELERATE_UPDATES_PER_SEC);

/**
 * @brief Accelerate both wheels at the same sinusoidal rate
 *
 * @param from_speed the starting speed
 * @param to_speed the ending speed
 * @param avg_accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_forward_sin(int from_speed, int to_speed, double avg_accel_per_sec = SIN_ACCEL_AVG_PER_SEC, int updates_per_sec = ACCELERATE_UPDATES_PER_SEC);

/**
 * @brief Accelerate both wheels at potentially different linear rates
 *
 * @param from_speed the starting speed, in the form of {left_speed, right_speed}
 * @param to_speed the ending speed, in the form of {left_speed, right_speed}
 * @param accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_linear(Speed from_speed, Speed to_speed, double accel_per_sec = LINEAR_ACCEL_PER_SEC, int updates_per_sec = ACCELERATE_UPDATES_PER_SEC);

/**
 * @brief Accelerate both wheels at potentially different sinusoidal rates
 *
 * @param from_speed the starting speed, in the form of {left_speed, right_speed}
 * @param to_speed the ending speed, in the form of {left_speed, right_speed}
 * @param accel_per_sec how much to accelerate per second (default 500)
 * @param updates_per_sec how many updates to the speed to make per second (default 500)
 */
void accelerate_sinusoidal(Speed from_speed, Speed to_speed, double accel_per_sec = SIN_ACCEL_AVG_PER_SEC, int updates_per_sec = ACCELERATE_UPDATES_PER_SEC);

#endif
/**@}*/