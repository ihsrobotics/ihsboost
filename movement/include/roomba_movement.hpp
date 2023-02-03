#ifndef ROOMBA_TURNS_HPP
#define ROOMBA_TURNS_HPP

#define ROOMBA_ACCEL_PER_SEC 500
#define ROOMBA_CORRECTION_PROPORTION .6
#define ROOMBA_UPDATES_PER_SEC 500
#define ROOMBA_MIN_SPEED 11

#define deg2rad 0.017453292519943296
#define DIST_BETWEEN_WHEEL 23.5

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
void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle);

/**
 * @brief
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

#endif