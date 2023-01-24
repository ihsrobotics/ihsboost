#include "gyro_movement.hpp"
#include "accelerator.hpp"
#include <kipr/wombat.h>

#define MEAN_VAL 3.5484280676588367
#define MIN_VAL 0
#define MAX_VAL 8
#define RAW_TO_360_DEGREES 4.6895
#define between(val, a, b) ((a >= val && val >= b) || (b >= val && val >= a))

double get_gyro_z_val()
{
    signed short val = gyro_z();
    return between(val, MIN_VAL, MAX_VAL) ? 0 : static_cast<double>(val) - MEAN_VAL;
}

void gyro_drive_straight(int from_speed, int to_speed, std::function<bool()> stop_function, double correction_proportion, double accel_per_sec, int updates_per_sec)
{
    LinearAccelerator accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);

    double accumulator = 0; // positive values = clockwise, negative values = CCW
    double speed = accelerator.speed();
    create_drive_direct(speed, speed);
    while (!stop_function())
    {
        speed = accelerator.speed();
        accumulator += get_gyro_z_val();
        if ((accumulator > 0 && speed > 0) || (accumulator < 0 && speed < 0))
        { // go slower on left wheel, faster on right wheel
            create_drive_direct(static_cast<int>(speed * correction_proportion), static_cast<int>(speed / correction_proportion));
        }
        else
        { // go faster on left wheel, slower on right wheel
            create_drive_direct(static_cast<int>(speed / correction_proportion), static_cast<int>(speed * correction_proportion));
        }
        accelerator.step();
        msleep(accelerator.get_msleep_time());
    }
}

void gyro_turn_degrees(Speed from_speed, Speed to_speed, int degrees, double accel_per_sec, int updates_per_sec)
{
    LinearAccelerator left_accelerator(from_speed.left, to_speed.left, accel_per_sec, updates_per_sec);
    LinearAccelerator right_accelerator(from_speed.right, to_speed.right, accel_per_sec, updates_per_sec);
    double accumulator = 0;
    while ((degrees > 0 && accumulator < degrees * RAW_TO_360_DEGREES) || (degrees < 0 && accumulator > degrees * RAW_TO_360_DEGREES))
    {
        accumulator += get_gyro_z_val() * 0.005;
        create_drive_direct(static_cast<int>(left_accelerator.speed()), static_cast<int>(right_accelerator.speed()));
        left_accelerator.step();
        right_accelerator.step();
        msleep(left_accelerator.get_msleep_time());
    }
    create_drive_direct(to_speed.left, to_speed.right);
}

void gyro_turn_degrees_v2(int max_speed, int degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // this function is meant to deal with 2 main turning cases:
    // 1 - where you accelerate to speed, have time to keep turning, then need to start
    // decelerating after you reach the final_angle - angle_turned_while_accelerating.
    // 2 - where you try to accelerate to speed, already reach half the angle, then need to start decelerating

    LinearAccelerator accelerator(0, max_speed, accel_per_sec, updates_per_sec);

    double accumulator = 0;
    double cached_accumulator = 0;
    double speed;
    int left_sign = (degrees > 0) ? 1 : -1;
    int right_sign = (degrees > 0) ? -1 : 1;

    // accelerating part of the turn, capped at 1/2 of the turn that way, if it didn't have enough time to accelerate,
    // it will still have enough time to decelerate from its current speed
    while ((degrees > 0 && accumulator < degrees * RAW_TO_360_DEGREES / 2) || (degrees < 0 && accumulator > degrees * RAW_TO_360_DEGREES / 2))
    {
        accumulator += get_gyro_z_val() * 0.005;
        speed = accelerator.speed();
        create_drive_direct(static_cast<int>(speed * left_sign), static_cast<int>(speed * right_sign));
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        if (accelerator.done() && cached_accumulator != 0)
        {
            cached_accumulator = accumulator;
            break;
        }
    }

    // do any extra turning that is needed
    while (cached_accumulator != 0 &&
           ((degrees > 0 && accumulator < degrees * RAW_TO_360_DEGREES - cached_accumulator) ||
            (degrees < 0 && accumulator > degrees * RAW_TO_360_DEGREES - cached_accumulator)))
    {
        accumulator += get_gyro_z_val() * 0.005;
        create_drive_direct(static_cast<int>(speed * left_sign), static_cast<int>(speed * right_sign));
        msleep(accelerator.get_msleep_time());
    }

    // decelerate from current speed to min_speed, which should be close to 0
    LinearAccelerator decelerator(speed, min_speed, accel_per_sec, updates_per_sec);

    while ((degrees > 0 && accumulator < degrees * RAW_TO_360_DEGREES) || (degrees < 0 && accumulator > degrees * RAW_TO_360_DEGREES))
    {
        accumulator += get_gyro_z_val() * 0.005;
        speed = decelerator.speed();
        create_drive_direct(static_cast<int>(speed * left_sign), static_cast<int>(speed * right_sign));
        decelerator.step();
        msleep(decelerator.get_msleep_time());
    }

    // stop at the end
    create_drive_direct(0, 0);
}