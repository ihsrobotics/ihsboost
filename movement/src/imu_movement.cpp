#include "imu_movement.hpp"
#include "controllers.hpp"
#include "accumulator.hpp"
#include "config.hpp"
#include <iostream>
#include <kipr/wombat.h>
using std::cout;
using std::endl;

#define between(val, a, b) ((a >= val && val >= b) || (b >= val && val >= a))

// --------------------------------------------- GYRO SECTION ---------------------------------------------
// Functions in this section deal with gyroscope things
// --------------------------------------------------------------------------------------------------------

double get_gyro_val()
{
    signed short val = GYRO_FUNCTION();
    return between(val, get_config().getDouble("min_gyro_val"), get_config().getDouble("max_gyro_val")) ? 0 : (static_cast<double>(val) - get_config().getDouble("mean_gyro_val")) * get_config().getDouble("gyro_cw_multiplier");
}

// helper function
void gyro_drive_straight_step(double &accumulator, double correction_proportion, double speed)
{
    accumulator += get_gyro_val();
    if ((accumulator > 0 && speed > 0) || (accumulator < 0 && speed < 0))
    { // go slower on left wheel, faster on right wheel
        MOVEMENT_FUNCTION(static_cast<int>(speed * correction_proportion), static_cast<int>(speed / correction_proportion));
    }
    else
    { // go faster on left wheel, slower on right wheel
        MOVEMENT_FUNCTION(static_cast<int>(speed / correction_proportion), static_cast<int>(speed * correction_proportion));
    }
}
// overload where you pass in the accelerator instead of the speed
void gyro_drive_straight_step(double &accumulator, double correction_proportion, AccelerateController &accelerator)
{
    gyro_drive_straight_step(accumulator, correction_proportion, accelerator.speed());
    accelerator.step();
    msleep(accelerator.get_msleep_time());
}

void gyro_drive_straight(int from_speed, int to_speed, std::function<bool()> stop_function, double correction_proportion, double accel_per_sec, int updates_per_sec)
{
    LinearController accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);

    double accumulator = 0; // positive values = clockwise, negative values = CCW
    MOVEMENT_FUNCTION(from_speed, from_speed);
    while (!stop_function())
    {
        gyro_drive_straight_step(accumulator, correction_proportion, accelerator);
    }
    MOVEMENT_FUNCTION(to_speed, to_speed);
}

void gyro_turn_degrees(Speed from_speed, Speed to_speed, int degrees, double accel_per_sec, int updates_per_sec)
{
    LinearController left_accelerator(from_speed.left, to_speed.left, accel_per_sec, updates_per_sec);
    LinearController right_accelerator(from_speed.right, to_speed.right, accel_per_sec, updates_per_sec);
    double accumulator = 0;
    double multiplier = static_cast<double>(left_accelerator.get_msleep_time()) / 1000.0;
    while ((degrees > 0 && accumulator < degrees * get_config().getDouble("raw_to_360_degrees")) || (degrees < 0 && accumulator > degrees * get_config().getDouble("raw_to_360_degrees")))
    {
        accumulator += get_gyro_val() * multiplier;
        MOVEMENT_FUNCTION(static_cast<int>(left_accelerator.speed()), static_cast<int>(right_accelerator.speed()));
        left_accelerator.step();
        right_accelerator.step();
        msleep(left_accelerator.get_msleep_time());
    }
    MOVEMENT_FUNCTION(to_speed.left, to_speed.right);
}

void gyro_turn_degrees_v2(int max_speed, int degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // this function is meant to deal with 2 main turning cases:
    // 1 - where you accelerate to speed, have time to keep turning, then need to start
    // decelerating after you reach the final_angle - angle_turned_while_accelerating.
    // 2 - where you try to accelerate to speed, already reach half the angle, then need to start decelerating

    LinearController accelerator(0, max_speed, accel_per_sec, updates_per_sec);

    Accumulator gyro_accumulator(get_gyro_val, 200);

    double cached_accumulator = 0;
    double speed;
    int left_sign = (degrees > 0) ? 1 : -1;
    int right_sign = (degrees > 0) ? -1 : 1;

    gyro_accumulator.start_accumulating();

    // accelerating part of the turn, capped at 1/2 of the turn that way, if it didn't have enough time to accelerate,
    // it will still have enough time to decelerate from its current speed
    while ((degrees > 0 && gyro_accumulator.get_accumulator() < degrees * get_config().getDouble("raw_to_360_degrees") / 2) ||
           (degrees < 0 && gyro_accumulator.get_accumulator() > degrees * get_config().getDouble("raw_to_360_degrees") / 2))
    {
        speed = accelerator.speed();
        MOVEMENT_FUNCTION(static_cast<int>(speed * left_sign), static_cast<int>(speed * right_sign));
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        if (accelerator.done())
        {
            cached_accumulator = gyro_accumulator.get_accumulator();
            break;
        }
    }

    // do any extra turning that is needed
    MOVEMENT_FUNCTION(static_cast<int>(speed * left_sign), static_cast<int>(speed * right_sign));
    while (cached_accumulator != 0 &&
           ((degrees > 0 && gyro_accumulator.get_accumulator() < degrees * get_config().getDouble("raw_to_360_degrees") - cached_accumulator) ||
            (degrees < 0 && gyro_accumulator.get_accumulator() > degrees * get_config().getDouble("raw_to_360_degrees") - cached_accumulator)))
    {
        msleep(accelerator.get_msleep_time());
    }

    // decelerate from current speed to min_speed, which should be close to 0
    LinearController decelerator(speed, min_speed, accel_per_sec, updates_per_sec);

    while ((degrees > 0 && gyro_accumulator.get_accumulator() < degrees * get_config().getDouble("raw_to_360_degrees")) ||
           (degrees < 0 && gyro_accumulator.get_accumulator() > degrees * get_config().getDouble("raw_to_360_degrees")))
    {
        speed = decelerator.speed();
        MOVEMENT_FUNCTION(static_cast<int>(speed * left_sign), static_cast<int>(speed * right_sign));
        decelerator.step();
        msleep(decelerator.get_msleep_time());
    }

    // stop at the end
    MOVEMENT_FUNCTION(0, 0);

    gyro_accumulator.stop_accumulating();
}
