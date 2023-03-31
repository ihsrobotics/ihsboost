/**
 * @file gyro_example.cpp
 * @author Eliot Hall
 * @brief File that demonstrates use of gyro functions
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <ihsboost/all.hpp>
#include <kipr/wombat.h>
#include <iostream>
using namespace std;

int main()
{
    create_connect_once();

    // go forward for 6 seconds, accelerating from 0 to 100 and checking for straightness using the gyroscope
    gyro_drive_straight(0, 100, Timer(6));
    // turn degrees using the gyroscope
    // turn 90 degrees by slowing down the right wheel
    // speeds are {left, right}, so we keep left wheel at speed 100 but gradually slow right wheel down to 11
    gyro_turn_degrees({100, 100}, {100, 11}, 90);

    // wait a little
    create_drive_direct(0, 0);
    msleep(500);

    // the other method of turning
    gyro_turn_degrees_v2(400, 360); // turn 360 degrees at max speed of 400

    create_disconnect();
    return 0;
}