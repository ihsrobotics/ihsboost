#ifndef ROOMBA_TURNS_HPP
#define ROOMBA_TURNS_HPP

#define ROOMBA_ACCEL_PER_SEC 500
#define ROOMBA_CORRECTION_PROPORTION .85
#define ROOMBA_UPDATES_PER_SEC 500
#define ROOMBA_MIN_SPEED 11

#define deg2rad 0.017453292519943296
#define rad2deg 57.29577951308232
#define DIST_BETWEEN_WHEEL 23.5 // in cm

// these change between roombas
#define LEFT_WHEEL_UNITS 0.11 // in cm / sec
#define RIGHT_WHEEL_UNITS 0.103

/**
 * @brief Rotate using speed-based turning on the roomba
 *
 * @param leftWheelSpeed - double - it is speed of left wheel in CM per second
 * @param rightWheelSpeed - double - it is speed of right wheel in CM per second
 * @param angle - double - angle to turn to in degrees
 */
void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle, double left_wheel_units = LEFT_WHEEL_UNITS, double right_wheel_units = RIGHT_WHEEL_UNITS);

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