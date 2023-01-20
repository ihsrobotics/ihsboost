#include "roomba_turns.hpp"
#include <kipr/wombat.h>
#include <stdlib.h>

void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle)
{
    // calculate curl to get the time when angle is reached
    double time = abs(DIST_BETWEEN_WHEEL / (rightWheelSpeed - leftWheelSpeed) *
                      angle * deg2rad * 1000);

    create_drive_direct(leftWheelSpeed / LEFT_WHEEL_UNITS,
                        rightWheelSpeed / RIGHT_WHEEL_UNITS);
    msleep(time);
    create_drive_direct(0, 0);
}