/**
 * @file roomba_movement.hpp
 * @author Eliot Hall
 * @brief Roomba-specific movement functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef ROOMBA_TURNS_HPP
#define ROOMBA_TURNS_HPP

#include "speed.hpp"
#include "config.hpp"
#include <functional>

#define deg2rad_mult 0.017453292519943296 ///< convert degrees to radians by multiplying by this
#define rad2deg_mult 57.29577951308232    ///< convert radians to degrees by multiplying by this
#define DIST_BETWEEN_WHEEL 23.5           ///< distance between the roomba wheels, in cm

#define ENC_2_MM (M_PI * 72.0 / 508.8)   ///< multiply by this to convert enc to mm
#define MM_2_ENC (508.8 / (72.0 * M_PI)) ///< multiply by this to convert mm to enc

/**
 * @brief Rotate using speed-based turning on the roomba
 *
 * @param leftWheelSpeed - double - it is speed of left wheel in CM per second. Use values from [-50, 50]
 * @param rightWheelSpeed - double - it is speed of right wheel in CM per second. Use values from [-50, 50]
 * @param angle - double - angle to turn to in degrees
 * @param left_wheel_units how far the left wheel moves, in cm/sec
 * @param right_wheel_units how far the right wheel moves, in cm/sec
 */
void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle, double left_wheel_units = get_config().getDouble("roomba_left_wheel_units"), double right_wheel_units = get_config().getDouble("roomba_right_wheel_units"));

/**
 * @brief Read create encoders into lenc and renc
 * @details This gives raw encoder values, which range from [-32768, 32767]
 *
 * @param[out] lenc the int that will store the left encoder value
 * @param[out] renc the int that will store the right encoder value
 */
void read_encoders(int &lenc, int &renc);

/**
 * @brief Process encoders so that, even if overflow/underflow occurs, `lenc_delta` and `renc_delta` will be accurate
 *
 * @param lenc_prev the previous left encoder value. This is modified inside the function
 *  to the value of the new reading
 * @param renc_prev the previous right encoder value. This is modified inside the function
 *  to the value of the new reading
 * @param lenc_delta the change in the left encoder. This is incremented inside the function
 * @param renc_delta the change in the right encoder. This is incremeneted inside the function
 */
void process_encoders(int &lenc_prev, int &renc_prev, int &lenc_delta, int &renc_delta);

/**
 * @brief Drive straight using create encoders
 * @details Uses the following formula: `N counts * (mm in 1 wheel revolution / counts in 1 wheel revolution) = mm`
 *
 * @param speed A positive value representing the absolute value of the max speed to travel at
 * @param cm how many centimeters to travel
 * @param stop whether or not to do a full stop after finishing
 * @param min_speed the minimum speed to travel at
 * @param correction_proportion how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
 * @param accel_per_sec how fast to accelerate per second
 * @param updates_per_sec how many updates to do per second
 * @return double - the number of mm driven
 */
double encoder_drive_straight(int speed, double cm, bool stop = get_config().getBool("roomba_stop"), int min_speed = get_config().getInt("roomba_min_speed"), double correction_proportion = get_config().getDouble("roomba_correction_proportion"), double accel_per_sec = get_config().getDouble("roomba_accel_per_sec"), int updates_per_sec = get_config().getInt("roomba_updates_per_sec"));

/**
 * @brief Drive straight at speed until it is time to stop
 *
 * @param speed the speed to drive at, can be positive or negative
 * @param condition a function that returns true when it is time to stop
 * @param stop whether or not to do a full stop after aligning
 * @param correction_proportion how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
 * @param updates_per_sec how many updates to do per second
 * @return double - the number of mm driven
 */
double encoder_drive_straight(int speed, std::function<bool()> condition, bool stop = get_config().getBool("roomba_stop"), double correction_proportion = get_config().getDouble("roomba_correction_proportion"), int updates_per_sec = get_config().getInt("roomba_updates_per_sec"));

/**
 * @brief Drive the create straight using create encoders and PID control (Proportional/Integral/Derivative)
 * @details Requires tuning of proportional, integral, and derivative coefficients for good results. \see PIDController
 *
 * @param speed the max speed to move at
 * @param cm how many centimeters to move
 * @param proportional_coefficient the coefficient for proportionality to the error
 * @param integral_coefficient the coefficient for the integral of the error
 * @param derivative_coefficient the coefficient for the derivative of the error
 * @param stop whether or not to do a full stop after aligning
 * @param min_speed the minimum speed to drive at
 * @param accel_per_sec how fast to accelerate per second
 * @param updates_per_second how many updates to do per second
 */
void encoder_drive_straight_pid(int speed, double cm, double proportional_coefficient, double integral_coefficient, double derivative_coefficient, bool stop = get_config().getBool("roomba_stop"), int min_speed = get_config().getInt("roomba_min_speed"), double accel_per_sec = get_config().getDouble("roomba_accel_per_sec"), int updates_per_second = get_config().getInt("roomba_updates_per_sec"));

/**
 * @brief Turns a certain number of degrees using create encoders
 * @details angle in radians = (left wheel distance – right wheel distance) / wheel base distance.
 *
 * @param max_speed The maximum speed to turn at. Should be greater than 0..
 * @param degrees The number of degrees to turn, positive values for CW
 * @param min_speed The minimum speed to turn at when decelerating
 * @param accel_per_sec How fast to accelerate per second
 * @param updates_per_sec How many updates to do per second
 */
void encoder_turn_degrees(int max_speed, double degrees, int min_speed = get_config().getInt("roomba_min_speed"), double accel_per_sec = get_config().getDouble("roomba_accel_per_sec"), int updates_per_sec = get_config().getInt("roomba_updates_per_sec"));

/**
 * @brief Turns a certain number of degrees using create encoders
 * @details This overload doesn't accelerate / decelerate; instead,
 * it merely turns at the given speed until reaching the goal degrees
 *
 * @param turn_speed The speed to turn at. \see Speed
 * @param degrees The number of degrees to turn, positive values for CW
 * @param updates_per_sec How many updates to do per second
 */
void encoder_turn_degrees(Speed turn_speed, double degrees, int updates_per_sec = get_config().getInt("roomba_updates_per_sec"));

#endif
/**@}*/