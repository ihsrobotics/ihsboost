#include "imu_movement.hpp"
#include "controllers.hpp"
#include "accumulator.hpp"
#include "config.hpp"
#include "singletons.hpp"
#include "maneuvers.hpp"
#include <kipr/wombat.h>

// --------------------------------------------- GYRO FUNCTIONS ---------------------------------------------
// Functions in this section deal with gyroscope functions
// --------------------------------------------------------------------------------------------------------

// helper function
void gyro_drive_straight_step(GyroSubscriber &subscriber, double correction_proportion, double speed)
{
    if ((subscriber.get_relative_angle() > 0 && speed > 0) || (subscriber.get_relative_angle() < 0 && speed < 0))
    { // go slower on left wheel, faster on right wheel
        MOVEMENT_FUNCTION(static_cast<int>(speed * correction_proportion), static_cast<int>(speed / correction_proportion));
    }
    else
    { // go faster on left wheel, slower on right wheel
        MOVEMENT_FUNCTION(static_cast<int>(speed / correction_proportion), static_cast<int>(speed * correction_proportion));
    }
}
// overload where you pass in the accelerator instead of the speed
void gyro_drive_straight_step(GyroSubscriber &subscriber, double correction_proportion, AccelerateController &accelerator)
{
    gyro_drive_straight_step(subscriber, correction_proportion, accelerator.speed());
    accelerator.step();
    msleep(accelerator.get_msleep_time());
}

void gyro_drive_straight(int from_speed, int to_speed, std::function<bool()> stop_function, double correction_proportion, double accel_per_sec, int updates_per_sec)
{
    LinearController accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);

    // setup gyro
    GyroSubscriber subscriber(updates_per_sec);

    MOVEMENT_FUNCTION(from_speed, from_speed);
    while (!stop_function())
    {
        gyro_drive_straight_step(subscriber, correction_proportion, accelerator);
    }
    MOVEMENT_FUNCTION(to_speed, to_speed);
}

void gyro_turn_degrees(Speed turn_speed, double degrees, int updates_per_sec)
{
    GyroSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, turn_speed, degrees, updates_per_sec);
}

void gyro_turn_degrees_v2(int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // setup gyro
    GyroSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, max_speed, degrees, min_speed, accel_per_sec, updates_per_sec);
}
