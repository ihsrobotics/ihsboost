#pragma once

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
