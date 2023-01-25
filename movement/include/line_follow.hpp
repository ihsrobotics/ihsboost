#pragma once

#include "cliff.hpp"
#include "accelerate.hpp"
#include <functional>

// greater than this value is white, less than is black
#define BLACK 2000
#define LINE_FOLLOW_UPDATES_PER_SEC 500
#define PROPORTIONAL_CONSTANT .90
#define LINE_FOLLOW_LINEAR_ACCELERATION 500
#define LINE_FOLLOW_SINUSOIDAL_ACCELERATION 500

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
