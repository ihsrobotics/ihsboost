/**
 * @file encoder_client_example.cpp
 * @author Eliot Hall
 * @brief File that demonstrates use of EncoderSubscriber
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <ihsboost/all.hpp>
#include <kipr/wombat.h>
#include <iostream>
#include <kipr/wombat.hpp>
using namespace std;

int main()
{
    create_connect_once();

    // set refresh rate of the create to once every 4 ms (default is 10 ms)
    kipr::create::Create::instance()->setRefreshRate(4);

    // align with black
    align_with_black(200, -50, Cliff::LCliff, Cliff::RCliff);
    msleep(200);

    // we will use this as our new zero point.
    // so, we create a subscriber that way we remember our angle
    // at this point in time and keep accumulating gyro values
    // while we do stuff (so that we will have accurate data to
    // return to our original angle)
    int updates_per_sec = 200;
    EncoderSubscriber subscriber(updates_per_sec);

    // do some moving
    create_drive_direct(100, 25);
    msleep(2000);

    // then, turn back to our original angle
    // since we set our zero point when we had aligned with the line,
    // this should return us back to that same angle.
    create_drive_direct(-100, 100);
    cout << "relative angle is " << subscriber.get_relative_angle() << endl;
    while (subscriber.get_relative_angle() > 0)
    {
        msleep(1);
    }
    create_drive_direct(0, 0);

    create_disconnect();
    return 0;
}