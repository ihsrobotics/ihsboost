/**
 * @file roomba_movement.hpp
 * @author Eliot Hall
 * @brief Roomba-specific movement functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @defgroup movement_id Movement Functions
 * @{
 */
#ifndef ROOMBA_TURNS_HPP
#define ROOMBA_TURNS_HPP

#define ROOMBA_ACCEL_PER_SEC 500         ///< default acceleration for the roomba
#define ROOMBA_CORRECTION_PROPORTION .85 ///< default correction for the roomba
#define ROOMBA_UPDATES_PER_SEC 500       ///< default updates per second
#define ROOMBA_MIN_SPEED 11              ///< default min speed

#define deg2rad 0.017453292519943296 ///< convert degrees to radians by multiplying by this
#define rad2deg 57.29577951308232    ///< convert radians to degrees by multiplying by this
#define DIST_BETWEEN_WHEEL 23.5      ///< distance between the roomba wheels, in cm

// these change between roombas
#define LEFT_WHEEL_UNITS 0.11   ///< how far the left wheel moves, in cm/sec
#define RIGHT_WHEEL_UNITS 0.103 ///< how far the right wheel moves, in cm/sec

/**
 * @brief Rotate using speed-based turning on the roomba
 *
 * @param leftWheelSpeed - double - it is speed of left wheel in CM per second. Use values from [-50, 50]
 * @param rightWheelSpeed - double - it is speed of right wheel in CM per second. Use values from [-50, 50]
 * @param angle - double - angle to turn to in degrees
 * @param left_wheel_units how far the left wheel moves, in cm/sec
 * @param right_wheel_units how far the right wheel moves, in cm/sec
 */
void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle, double left_wheel_units = LEFT_WHEEL_UNITS, double right_wheel_units = RIGHT_WHEEL_UNITS);

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
 * @param min_speed the minimum speed to travel at
 * @param correction_proportion how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
 * @param accel_per_sec how fast to accelerate per second
 * @param updates_per_sec how many updates to do per second
 */
void encoder_drive_straight(int speed, double cm, int min_speed = ROOMBA_MIN_SPEED, double correction_proportion = ROOMBA_CORRECTION_PROPORTION, double accel_per_sec = ROOMBA_ACCEL_PER_SEC, int updates_per_sec = ROOMBA_UPDATES_PER_SEC);

/**
 * @brief Drive the create straight using create encoders and pid
 *
 * @param speed the max speed to move at
 * @param cm how many centimeters to move
 * @param proportional_coefficient the coefficient for proportionality to the error
 * @param integral_coefficient the coefficient for the integral of the error
 * @param derivative_coefficient the coefficient for the derivative of the error
 * @param min_speed the minimum speed to drive at
 * @param accel_per_sec how fast to accelerate per second
 * @param updates_per_second how many updates to do per second
 */
void encoder_drive_straight_pid(int speed, double cm, double proportional_coefficient, double integral_coefficient, double derivative_coefficient, int min_speed, double accel_per_sec = ROOMBA_ACCEL_PER_SEC, int updates_per_second = ROOMBA_UPDATES_PER_SEC);

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
void encoder_turn_degrees(int max_speed, int degrees, int min_speed = ROOMBA_MIN_SPEED, double accel_per_sec = ROOMBA_ACCEL_PER_SEC, int updates_per_sec = ROOMBA_UPDATES_PER_SEC);

#endif
/**@}*/