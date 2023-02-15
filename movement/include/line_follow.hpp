/**
 * @file line_follow.hpp
 * @author Eliot Hall
 * @brief Line follow functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef LINE_FOLLOW_HPP
#define LINE_FOLLOW_HPP

#include "cliff.hpp"
#include "accelerate.hpp"
#include <functional>

#define BLACK 2000                              ///< greater than this value is white, less than is black
#define LINE_FOLLOW_UPDATES_PER_SEC 500         ///< default updates per second when line following
#define PROPORTIONAL_CONSTANT .90               ///< default correction
#define LINE_FOLLOW_LINEAR_ACCELERATION 500     ///< default linear acceleration
#define LINE_FOLLOW_SINUSOIDAL_ACCELERATION 500 ///< default sinusoidal acceleration

#define ALIGN_UPDATES_PER_SEC 500 ///< default updates per second when aligning

/**
 * @brief Returns whether or not a sensor reading is black
 *
 * @param val the sensor value
 * @return true if it is black
 * @return false if it isn't black
 */
bool is_black(int val);

/**
 * @brief Returns whether or not a sensor reading is white
 *
 * @param val the sensor value
 * @return true if it is white
 * @return false if it isn't white
 */
bool is_white(int val);

/**
 * @brief Line follow at a given speed
 *
 * @param speed the speed to line follow at
 * @param cliff_sensor Which cliff sensor to use \see Cliff
 * @param line_side which side of the line to follow at \see LineSide
 * @param stop_condition a function that returns true when it is time to stop line-following
 * @param correction_proportion how large the correction should be while line following, defaults to .90,
 * should be between (0, 1)
 * @param black_val value for black, defaults to 2000
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void line_follow_basic(int speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, double correction_proportion = PROPORTIONAL_CONSTANT, int black_val = BLACK, int updates_per_sec = LINE_FOLLOW_UPDATES_PER_SEC);

/**
 * @brief Line follow, but accelerate linearly
 *
 * @param from_speed speed to start at
 * @param to_speed speed to finish line-following at
 * @param cliff_sensor which cliff sensor to use \see Cliff
 * @param line_side which side of the line to follow at \see LineSide
 * @param stop_condition a function that returns true when it is time to stop line-following
 * @param correction_proportion how large the correction should be while line following, defaults to .90,
 * should be between (0, 1)
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param black_val value for black, defaults to 2000
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void line_follow_accelerate_linear(int from_speed, int to_speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, double correction_proportion = PROPORTIONAL_CONSTANT, int accel_per_sec = LINE_FOLLOW_LINEAR_ACCELERATION, int black_val = BLACK, int updates_per_sec = LINE_FOLLOW_UPDATES_PER_SEC);

/**
 * @brief Line follow, but accelerate sinusoidally
 *
 * @param from_speed speed to start at
 * @param to_speed speed to finish line-following at
 * @param cliff_sensor which cliff sensor to use \see Cliff
 * @param line_side which side of the line to follow at \see LineSide
 * @param stop_condition a function that returns true when it is time to stop line-following
 * @param correction_proportion how large the correction should be while line following, defaults to .90,
 * should be between (0, 1)
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param black_val value for black, defaults to 2000
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void line_follow_accelerate_sinusoidal(int from_speed, int to_speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, double correction_proportion = PROPORTIONAL_CONSTANT, int accel_per_sec = LINE_FOLLOW_SINUSOIDAL_ACCELERATION, int black_val = BLACK, int updates_per_sec = LINE_FOLLOW_UPDATES_PER_SEC);

/**
 * @brief Align with black, meaning keep moving until both `cliff_sensor_l` and `cliff_sensor_r` are on black.
 *
 * @param speed the speed to go when not on black
 * @param correction_speed the speed to go at once on black
 * @param cliff_sensor_l the left cliff sensor to use
 * @param cliff_sensor_r the right cliff sensor to use
 * @param updates_per_second how many updates to do per second.
 */
void align_with_black(int speed, int correction_speed, Cliff cliff_sensor_l, Cliff cliff_sensor_r, int updates_per_second = ALIGN_UPDATES_PER_SEC);

/**
 * @brief Align with white, meaning keep moving until both `cliff_sensor_l` and `cliff_sensor_r` are on white.
 *
 * @param speed the speed to go when not on white
 * @param correction_speed the speed to go at once on white
 * @param cliff_sensor_l the left cliff sensor to use
 * @param cliff_sensor_r the right cliff sensor to use
 * @param updates_per_second how many updates to do per second.
 */
void align_with_white(int speed, int correction_speed, Cliff cliff_sensor_l, Cliff cliff_sensor_r, int updates_per_second = ALIGN_UPDATES_PER_SEC);

#endif
/**@}*/