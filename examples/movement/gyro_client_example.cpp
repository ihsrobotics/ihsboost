/**
 * @file gyro_client_example.cpp
 * @author Eliot Hall
 * @brief File that demonstrates use of GyroSubscriber
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

    // align with black
    align_with_black(200, -50, Cliff::LCliff, Cliff::RCliff);
    msleep(500);

    // we will use this as our new zero point.
    // so, we create a subscriber that way we remember our angle
    // at this point in time and keep accumulating gyro values
    // while we do stuff (so that we will have accurate data to
    // return to our original angle)
    int updates_per_sec = 200;
    GyroSubscriber subscriber(updates_per_sec);

    // do some moving
    create_drive_direct(100, 75);
    msleep(500);
    gyro_turn_degrees_v2(200, 90); // turn 90 degrees CW

    // then, turn back to our original angle
    // since we set our zero point when we had aligned with the line,
    // this should return us back to that same angle.
    create_drive_direct(-100, 100);
    while (subscriber.get_relative_angle() > 0)
    {
        msleep(1);
    }
    create_drive_direct(0, 0);

    create_disconnect();
    return 0;
}