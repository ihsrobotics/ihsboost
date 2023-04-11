#include "maneuvers.hpp"
#include "ihsboost/controllers/controllers.hpp"
#include "ihsboost/util/config.hpp"
#include <kipr/wombat.h>

void turn_degrees(Subscriber *subscriber, int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // initialize misc
    double cached_angle_degrees = 0;
    int left_sign_val = degrees > 0 ? 1 : -1;
    int right_sign_val = degrees > 0 ? -1 : 1;
    LinearController accelerator(0, max_speed, accel_per_sec, updates_per_sec);

    // turn until we've either reached halfway point (it's time to start decelerating)
    // or until the accelerator is done (meaning it's time to continue driving until it's time
    // to start decelerating)
    while ((degrees > 0 && subscriber->get_relative_angle() < degrees / 2) ||
           (degrees < 0 && subscriber->get_relative_angle() > degrees / 2))
    {
        MOVEMENT_FUNCTION(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        if (accelerator.done())
        {
            cached_angle_degrees = subscriber->get_relative_angle();
            break;
        }
    }

    // do any more driving until it is time to start decelerating)
    while (cached_angle_degrees != 0 &&
           ((degrees > 0 && subscriber->get_relative_angle() < degrees - cached_angle_degrees) ||
            (degrees < 0 && subscriber->get_relative_angle() > degrees - cached_angle_degrees)))
    {
        MOVEMENT_FUNCTION(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        msleep(accelerator.get_msleep_time());
    }

    // start decelerating, go until both lenc and renc have reached the end
    LinearController decelerator(static_cast<int>(accelerator.speed()), min_speed, accel_per_sec, updates_per_sec);
    while ((degrees > 0 && subscriber->get_relative_angle() < degrees) ||
           (degrees < 0 && subscriber->get_relative_angle() > degrees))

    {
        // both still have places to go
        MOVEMENT_FUNCTION(static_cast<int>(decelerator.speed() * left_sign_val), static_cast<int>(decelerator.speed() * right_sign_val));

        // sleep
        decelerator.step();
        msleep(decelerator.get_msleep_time());
    }

    // stop at the end
    MOVEMENT_FUNCTION(0, 0);
}

void turn_degrees(Subscriber *subscriber, Speed turn_speed, double degrees, int updates_per_sec)
{
    // drive until reached goal degrees
    MOVEMENT_FUNCTION(turn_speed.left, turn_speed.right);
    while ((degrees > 0 && subscriber->get_relative_angle() < degrees) || (degrees < 0 && subscriber->get_relative_angle() > degrees))
    {
        // sleep
        msleep(1000 / updates_per_sec);
    }

    // stop
    MOVEMENT_FUNCTION(0, 0);
}

void drive_straight(Subscriber *subscriber, int speed, std::function<bool()> condition, bool stop, double correction_proportion, int updates_per_sec)
{
    // go until told to stop
    while (!condition())
    {
        if ((speed > 0 && subscriber->get_relative_angle() > 0) || (speed < 0 && subscriber->get_relative_angle() < 0))
        {
            MOVEMENT_FUNCTION(static_cast<int>(speed * correction_proportion), static_cast<int>(speed / correction_proportion));
        }
        // if right wheel going faster, go slower
        else
        {
            MOVEMENT_FUNCTION(static_cast<int>(speed / correction_proportion), static_cast<int>(speed * correction_proportion));
        }
        msleep(1000 / updates_per_sec);
    }

    // stop at the end
    if (stop)
    {
        MOVEMENT_FUNCTION(0, 0);
    }
    else
    {
        MOVEMENT_FUNCTION(speed, speed);
    }
}
