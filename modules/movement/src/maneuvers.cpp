#include "maneuvers.hpp"
#include "ihsboost/controllers/controllers.hpp"
#include "ihsboost/util/config.hpp"
#include <kipr/wombat.h>

void turn_degrees(AngleSubscriber *subscriber, int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // initialize misc
    int left_sign_val = degrees > 0 ? 1 : -1;
    int right_sign_val = degrees > 0 ? -1 : 1;
    THREE_PART_LOOP(max_speed, min_speed, accel_per_sec, updates_per_sec, MOVEMENT_FUNCTION(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val)), MOVEMENT_FUNCTION(static_cast<int>(decelerator.speed() * left_sign_val), static_cast<int>(decelerator.speed() * right_sign_val)), degrees > 0 && subscriber->get_relative_angle() < degrees, degrees < 0 && subscriber->get_relative_angle() > degrees, subscriber->get_relative_angle())
    MOVEMENT_FUNCTION(0, 0);
}

void turn_degrees(AngleSubscriber *subscriber, Speed turn_speed, double degrees, int updates_per_sec)
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

void drive_straight(AngleSubscriber *subscriber, int speed, std::function<bool()> condition, bool stop, double correction_proportion, int updates_per_sec)
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
