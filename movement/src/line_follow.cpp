#include <kipr/wombat.h>
#include "line_follow.hpp"
#include "accelerator.hpp"

std::function<int()> get_sensor(Cliff cliff_sensor)
{
    switch (cliff_sensor)
    {
    case LCliff:
        return get_create_lcliff_amt;
    case LFCliff:
        return get_create_lfcliff_amt;
    case RFCliff:
        return get_create_rfcliff_amt;
    case RCliff:
        return get_create_rcliff_amt;
    default:
        throw "INVALID CLIFF SENSOR PROVIDED";
    }
}

void line_follow_accelerate(Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, AccelerateController &accelerator, double correction_proportion, int black_val)
{
    std::function<int()> sensor = get_sensor(cliff_sensor);
    int cur_val;

    create_drive_direct(static_cast<int>(accelerator.speed()), static_cast<int>(accelerator.speed()));
    while (!stop_condition())
    {
        cur_val = sensor();
        if (line_side == LeftSide)
        {
            if (cur_val > black_val)
            { // on white
                create_drive_direct(static_cast<int>(accelerator.speed() / correction_proportion), static_cast<int>(accelerator.speed() * correction_proportion));
            }
            else
            { // on black
                create_drive_direct(static_cast<int>(accelerator.speed() * correction_proportion), static_cast<int>(accelerator.speed() / correction_proportion));
            }
        }
        else
        { // line side == RightSide
            if (cur_val > black_val)
            { // on white
                create_drive_direct(static_cast<int>(accelerator.speed() * correction_proportion), static_cast<int>(accelerator.speed() / correction_proportion));
            }
            else
            { // on black
                create_drive_direct(static_cast<int>(accelerator.speed() / correction_proportion), static_cast<int>(accelerator.speed() * correction_proportion));
            }
        }
        accelerator.step();
        msleep(accelerator.get_msleep_time());
    }
    create_drive_direct(static_cast<int>(accelerator.speed()), static_cast<int>(accelerator.speed()));
}

void line_follow_basic(int speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, double correction_proportion, int black_val, int updates_per_sec)
{
    LinearAccelerator accelerator(speed, speed, 0, updates_per_sec);
    line_follow_accelerate(cliff_sensor, line_side, stop_condition, accelerator, correction_proportion, black_val);
}

void line_follow_accelerate_linear(int from_speed, int to_speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, double correction_proportion, int accel_per_sec, int black_val, int updates_per_sec)
{
    LinearAccelerator accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);
    line_follow_accelerate(cliff_sensor, line_side, stop_condition, accelerator, correction_proportion, black_val);
}

void line_follow_accelerate_sinusoidal(int from_speed, int to_speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, double correction_proportion, int accel_per_sec, int black_val, int updates_per_sec)
{
    SinusoidalAccelerator accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);
    line_follow_accelerate(cliff_sensor, line_side, stop_condition, accelerator, correction_proportion, black_val);
}
