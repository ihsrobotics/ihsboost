#include "accelerate.hpp"
#include "accelerator.hpp"
#include <kipr/wombat.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

void accelerate_forward_linear(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec)
{
    accelerate_linear({from_speed, from_speed}, {to_speed, to_speed}, accel_per_sec, updates_per_sec);
}

void accelerate_forward_sin(int from_speed, int to_speed, double avg_accel_per_sec, int updates_per_sec)
{
    accelerate_sinusoidal({from_speed, from_speed}, {to_speed, to_speed}, avg_accel_per_sec, updates_per_sec);
}

void accelerate_linear(Speed from_speed, Speed to_speed, double accel_per_sec, int updates_per_sec)
{
    Speed delta_speed = to_speed - from_speed;
    int greatest_delta = abs(delta_speed.left) > abs(delta_speed.right) ? abs(delta_speed.left) : abs(delta_speed.right);

    // make the controllers
    // maintain the proportion throughout the acceleration.
    // ex: if it was supposed to accelerate from 0,0 to 400,100, then this will attempt to maintain that 4:1 ratio
    // by accelerating the smaller one at a proportional rate (hence delta_speed / greatest_delta *accel_per_sec)
    LinearAccelerator left_accelerator(from_speed.left, to_speed.left,
                                       abs(delta_speed.left) == greatest_delta ? accel_per_sec : abs(static_cast<double>(delta_speed.left) / greatest_delta * accel_per_sec), updates_per_sec);
    LinearAccelerator right_accelerator(from_speed.right, to_speed.right,
                                        abs(delta_speed.right) == greatest_delta ? accel_per_sec : abs(static_cast<double>(delta_speed.right) / greatest_delta * accel_per_sec), updates_per_sec);

    // loop till done
    while (!left_accelerator.done() || !right_accelerator.done())
    {
        left_accelerator.step();
        right_accelerator.step();
        create_drive_direct(left_accelerator.speed(), right_accelerator.speed());
        msleep(left_accelerator.get_msleep_time()); // msleep time will be the same for both, so just use left
    }
    create_drive_direct(to_speed.left, to_speed.right);
}

void accelerate_sinusoidal(Speed from_speed, Speed to_speed, double accel_per_sec, int updates_per_sec)
{
    Speed delta_speed = to_speed - from_speed;
    int greatest_delta = abs(delta_speed.left) > abs(delta_speed.right) ? abs(delta_speed.left) : abs(delta_speed.right);

    // make the accelerators
    // try to maintain the proportion throughout the acceleration.
    // ex: if it was supposed to accelerate from 0,0 to 400,100, then this will attempt to maintain that 4:1 ratio
    // by accelerating the smaller one at a proportional rate (hence delta_speed / greatest_delta *accel_per_sec)
    // won't maintain the ratio perfectly during acceleration when using sinusoids, but will be closer than not
    // trying at all
    SinusoidalAccelerator left_accelerator(from_speed.left, to_speed.left,
                                           abs(delta_speed.left) == greatest_delta ? accel_per_sec : abs(static_cast<double>(delta_speed.left) / greatest_delta * accel_per_sec), updates_per_sec);
    SinusoidalAccelerator right_accelerator(from_speed.right, to_speed.right,
                                            abs(delta_speed.right) == greatest_delta ? accel_per_sec : abs(static_cast<double>(delta_speed.right) / greatest_delta * accel_per_sec), updates_per_sec);

    // loop till done
    while (!left_accelerator.done() || !right_accelerator.done())
    {
        left_accelerator.step();
        right_accelerator.step();
        create_drive_direct(left_accelerator.speed(), right_accelerator.speed());
        msleep(left_accelerator.get_msleep_time()); // msleep time will be the same for both, so just use left
    }
    create_drive_direct(to_speed.left, to_speed.right);
}