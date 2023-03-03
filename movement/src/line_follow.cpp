#include <kipr/wombat.h>
#include "line_follow.hpp"
#include "controllers.hpp"

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

bool is_black(int val)
{
    return val <= BLACK;
}

bool is_white(int val)
{
    return val > BLACK;
}

void line_follow_accelerate(Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, AccelerateController &accelerator, bool stop, double correction_proportion, int black_val)
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
    if (stop)
    {
        create_drive_direct(0, 0);
    }
    else
    {
        create_drive_direct(static_cast<int>(accelerator.speed()), static_cast<int>(accelerator.speed()));
    }
}

void line_follow_basic(int speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, bool stop, double correction_proportion, int black_val, int updates_per_sec)
{
    LinearController accelerator(speed, speed, 0, updates_per_sec);
    line_follow_accelerate(cliff_sensor, line_side, stop_condition, accelerator, stop, correction_proportion, black_val);
}

void line_follow_accelerate_linear(int from_speed, int to_speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, bool stop, double correction_proportion, int accel_per_sec, int black_val, int updates_per_sec)
{
    LinearController accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);
    line_follow_accelerate(cliff_sensor, line_side, stop_condition, accelerator, stop, correction_proportion, black_val);
}

void line_follow_accelerate_sinusoidal(int from_speed, int to_speed, Cliff cliff_sensor, LineSide line_side, std::function<bool()> stop_condition, bool stop, double correction_proportion, int accel_per_sec, int black_val, int updates_per_sec)
{
    SinusoidalController accelerator(from_speed, to_speed, accel_per_sec, updates_per_sec);
    line_follow_accelerate(cliff_sensor, line_side, stop_condition, accelerator, stop, correction_proportion, black_val);
}

void align(std::function<bool(int)> condition, int speed, int correction_speed, Cliff cliff_sensor_l, Cliff cliff_sensor_r, bool stop, int updates_per_second)
{
    std::function<int()> l_sensor = get_sensor(cliff_sensor_l);
    std::function<int()> r_sensor = get_sensor(cliff_sensor_r);

    while (condition(l_sensor()) || condition(r_sensor()))
    {
        // both still need to keep going
        if (condition(l_sensor()) && condition(r_sensor()))
        {
            create_drive_direct(speed, speed);
        }
        else if (condition(l_sensor())) // left still has places to go
        {
            create_drive_direct(speed, correction_speed);
        }
        else // right still has places to go
        {
            create_drive_direct(correction_speed, speed);
        }

        msleep(1000 / updates_per_second);
    }

    if (stop)
    {
        create_drive_direct(0, 0);
    }
    else
    {
        create_drive_direct(speed, speed);
    }
}

void align_with_black(int speed, int correction_speed, Cliff cliff_sensor_l, Cliff cliff_sensor_r, bool stop, int updates_per_second)
{
    align(is_white, speed, correction_speed, cliff_sensor_l, cliff_sensor_r, stop, updates_per_second);
}

void align_with_white(int speed, int correction_speed, Cliff cliff_sensor_l, Cliff cliff_sensor_r, bool stop, int updates_per_second)
{
    align(is_black, speed, correction_speed, cliff_sensor_l, cliff_sensor_r, stop, updates_per_second);
}
