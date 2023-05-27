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
#ifndef IHSBOOST_LINE_FOLLOW_HPP
#define IHSBOOST_LINE_FOLLOW_HPP

#include "accelerate.hpp"
#include "ihsboost/util/cliff.hpp"
#include "ihsboost/util/config.hpp"
#include <functional>

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
 * @param stop_condition a function that returns true when it is time to stop
 * line-following
 * @param stop whether or not to do a full stop after finishing
 * @param correction_proportion how large the correction should be while line
 * following, defaults to .90, should be between (0, 1)
 * @param black_val value for black, defaults to 2000
 * @param updates_per_sec how many updates the function will do per sec,
 * defaults to 200
 */
void line_follow_basic(
    int speed,
    Cliff cliff_sensor,
    LineSide line_side,
    std::function<bool()> stop_condition,
    bool stop = get_config().getBool("line_follow_stop"),
    double correction_proportion =
        get_config().getDouble("line_follow_correction_proportion"),
    int black_val = get_config().getInt("black"),
    int updates_per_sec = get_config().getInt("line_follow_updates_per_sec"));

/**
 * @brief Line follow, but accelerate linearly
 *
 * @param from_speed speed to start at
 * @param to_speed speed to finish line-following at
 * @param cliff_sensor which cliff sensor to use \see Cliff
 * @param line_side which side of the line to follow at \see LineSide
 * @param stop_condition a function that returns true when it is time to stop
 * line-following
 * @param stop whether or not to do a full stop after finishing
 * @param correction_proportion how large the correction should be while line
 * following, defaults to .90, should be between (0, 1)
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param black_val value for black, defaults to 2000
 * @param updates_per_sec how many updates the function will do per sec,
 * defaults to 200
 */
void line_follow_accelerate_linear(
    int from_speed,
    int to_speed,
    Cliff cliff_sensor,
    LineSide line_side,
    std::function<bool()> stop_condition,
    bool stop = get_config().getBool("line_follow_stop"),
    double correction_proportion =
        get_config().getDouble("line_follow_correction_proportion"),
    double accel_per_sec =
        get_config().getDouble("line_follow_linear_acceleration"),
    int black_val = get_config().getInt("black"),
    int updates_per_sec = get_config().getInt("line_follow_updates_per_sec"));

/**
 * @brief Line follow, but accelerate sinusoidally
 *
 * @param from_speed speed to start at
 * @param to_speed speed to finish line-following at
 * @param cliff_sensor which cliff sensor to use \see Cliff
 * @param line_side which side of the line to follow at \see LineSide
 * @param stop_condition a function that returns true when it is time to stop
 * line-following
 * @param stop whether or not to do a full stop after finishing
 * @param correction_proportion how large the correction should be while line
 * following, defaults to .90, should be between (0, 1)
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param black_val value for black, defaults to 2000
 * @param updates_per_sec how many updates the function will do per sec,
 * defaults to 200
 */
void line_follow_accelerate_sinusoidal(
    int from_speed,
    int to_speed,
    Cliff cliff_sensor,
    LineSide line_side,
    std::function<bool()> stop_condition,
    bool stop = get_config().getBool("line_follow_stop"),
    double correction_proportion =
        get_config().getDouble("line_follow_correction_proportion"),
    double accel_per_sec =
        get_config().getDouble("line_follow_sinusoidal_aceleration"),
    int black_val = get_config().getInt("black"),
    int updates_per_sec = get_config().getInt("line_follow_updates_per_sec"));

/**
 * @brief Align with black, meaning keep moving until both `cliff_sensor_l` and
 * `cliff_sensor_r` are on black.
 *
 * @param speed the speed to go when not on black
 * @param correction_speed the speed to go at once on black
 * @param cliff_sensor_l the left cliff sensor to use
 * @param cliff_sensor_r the right cliff sensor to use
 * @param stop whether or not to do a full stop after aligning
 * @param updates_per_second how many updates to do per second.
 */
void align_with_black(
    int speed,
    int correction_speed,
    Cliff cliff_sensor_l,
    Cliff cliff_sensor_r,
    bool stop = get_config().getBool("align_stop"),
    int updates_per_second = get_config().getInt("align_updates_per_sec"));

/**
 * @brief Align with white, meaning keep moving until both `cliff_sensor_l` and
 * `cliff_sensor_r` are on white.
 *
 * @param speed the speed to go when not on white
 * @param correction_speed the speed to go at once on white
 * @param cliff_sensor_l the left cliff sensor to use
 * @param cliff_sensor_r the right cliff sensor to use
 * @param stop whether or not to do a full stop after aligning
 * @param updates_per_second how many updates to do per second.
 */
void align_with_white(
    int speed,
    int correction_speed,
    Cliff cliff_sensor_l,
    Cliff cliff_sensor_r,
    bool stop = get_config().getBool("align_stop"),
    int updates_per_second = get_config().getInt("align_updates_per_sec"));

#endif
/**@}*/