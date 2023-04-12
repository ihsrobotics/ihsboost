#include "imu_movement.hpp"
#include "ihsboost/controllers/controllers.hpp"
#include "singletons.hpp"
#include "maneuvers.hpp"
#include <kipr/wombat.h>

// --------------------------------------------- GYRO FUNCTIONS ---------------------------------------------
// Functions in this section deal with gyroscope functions
// --------------------------------------------------------------------------------------------------------

void gyro_drive_straight(int speed, std::function<bool()> stop_function, bool stop, double correction_proportion, int updates_per_sec)
{
    // setup gyro
    GyroSubscriber subscriber(updates_per_sec);

    drive_straight(&subscriber, speed, stop_function, stop, correction_proportion, updates_per_sec);
}

void gyro_turn_degrees(Speed turn_speed, double degrees, int updates_per_sec)
{
    GyroSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, turn_speed, degrees, updates_per_sec);
}

void gyro_turn_degrees(int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // setup gyro
    GyroSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, max_speed, degrees, min_speed, accel_per_sec, updates_per_sec);
}
