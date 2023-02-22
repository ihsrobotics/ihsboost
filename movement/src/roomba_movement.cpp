#include "roomba_movement.hpp"
#include "controllers.hpp"
#include <kipr/wombat.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits>
#define ENC_2_MM (M_PI * 72.0 / 508.8)   // multiply by this to convert enc to mm
#define MM_2_ENC (508.8 / (72.0 * M_PI)) // multiply by this to convert mm to enc
using namespace std;

void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle, double left_wheel_units, double right_wheel_units)
{
    // calculate curl to get the time when angle is reached
    double time = abs(DIST_BETWEEN_WHEEL / (rightWheelSpeed - leftWheelSpeed) *
                      angle * deg2rad_mult * 1000.0);

    create_drive_direct(static_cast<int>(leftWheelSpeed / left_wheel_units),
                        static_cast<int>(rightWheelSpeed / right_wheel_units));
    msleep(time);
    create_drive_direct(0, 0);
}

void read_encoders(int &lenc, int &renc)
{
    // read values
    int16_t l_temp = 0, r_temp = 0;
    _create_get_raw_encoders(&l_temp, &r_temp);

    // set values
    lenc = l_temp;
    renc = r_temp;
}

void check_overflow(int16_t &temp, int &enc_prev, int &enc_delta)
{
    // this function aims to stop the effects of overflow
    // it detects overflow with the following:
    // check if this jumped a huge amount (sign of overflow)
    // the .8 is an arbitrary number just to make it more accepting

    // check if it overflowed
    if (abs(static_cast<int>(temp) - enc_prev) > std::numeric_limits<u_short>::max() * .8)
    {
        // attempt to correct it
        // case of underflow
        if (enc_prev < 0 && temp > 0)
        {
            // subtract the positive difference that it traveled on the positive side of the underflow
            enc_delta -= std::numeric_limits<int16_t>::max() - temp;
            // subtract the difference that it traveled on the negative side of the underflow
            enc_delta -= enc_prev - std::numeric_limits<int16_t>::min();
        }
        // case of overflow
        if (enc_prev > 0 && temp < 0)
        {
            // add the positive difference that it traveled on the positive side of the overflow
            enc_delta += std::numeric_limits<int16_t>::max() - enc_prev;
            // add the positive difference that it traveled on the negative side of the underflow
            enc_delta += temp - std::numeric_limits<int16_t>::min();
        }
    }
    else
    {
        // if didn't overflow, just continue as normal
        enc_delta += temp - enc_prev;
    }
}

void process_encoders(int &lenc_prev, int &renc_prev, int &lenc_delta, int &renc_delta)
{
    // read values
    int16_t l_temp = 0, r_temp = 0;
    _create_get_raw_encoders(&l_temp, &r_temp);

    check_overflow(l_temp, lenc_prev, lenc_delta);
    check_overflow(r_temp, renc_prev, renc_delta);

    // set the "previous" variables to the new previous values
    lenc_prev = l_temp;
    renc_prev = r_temp;
}

void encoder_drive_straight(int max_speed, double cm, int min_speed, double correction_proportion, double accel_per_sec, int updates_per_sec)
{
    // initialize misc
    double cached_distance = 0;
    const double mm = cm * 10;
    int sign_val = cm > 0 ? 1 : -1;

    // initialize encoder variables
    int lenc_prev = 0, renc_prev = 0, lenc_delta = 0, renc_delta = 0;
    read_encoders(lenc_prev, renc_prev);

    LinearController accelerator(0, max_speed * sign_val, accel_per_sec, updates_per_sec);

    while ((mm > 0 && (lenc_delta * ENC_2_MM < mm / 2 &&
                       renc_delta * ENC_2_MM < mm / 2)) ||
           (mm < 0 && (lenc_delta * ENC_2_MM > mm / 2 &&
                       renc_delta * ENC_2_MM > mm / 2)))
    {
        // if left wheel going faster, go slower
        if ((mm > 0 && lenc_delta > renc_delta) || (mm < 0 && lenc_delta < renc_delta))
        {
            create_drive_direct(static_cast<int>(accelerator.speed() * correction_proportion), static_cast<int>(accelerator.speed() / correction_proportion));
        }
        // if right wheel going faster, go slower
        else
        {
            create_drive_direct(static_cast<int>(accelerator.speed() / correction_proportion), static_cast<int>(accelerator.speed() * correction_proportion));
        }

        // sleep
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);

        if (accelerator.done())
        {
            cached_distance = (lenc_delta * ENC_2_MM + renc_delta * ENC_2_MM) / 2.0;
            break;
        }
    }

    // do any more driving until it is time to start decelerating
    while (cached_distance != 0 &&
           ((mm > 0 && (lenc_delta * ENC_2_MM < mm - cached_distance &&
                        renc_delta * ENC_2_MM < mm - cached_distance)) ||
            (mm < 0 && (lenc_delta * ENC_2_MM > mm - cached_distance &&
                        renc_delta * ENC_2_MM > mm - cached_distance))))
    {
        // if left wheel going faster, go slower
        if ((mm > 0 && lenc_delta > renc_delta) || (mm < 0 && lenc_delta < renc_delta))
        {
            create_drive_direct(static_cast<int>(accelerator.speed() * correction_proportion), static_cast<int>(accelerator.speed() / correction_proportion));
        }
        // if right wheel going faster, go slower
        else
        {
            create_drive_direct(static_cast<int>(accelerator.speed() / correction_proportion), static_cast<int>(accelerator.speed() * correction_proportion));
        }

        // sleep
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
    }

    // start decelerating, go until both lenc and renc have reached the end
    LinearController decelerator(accelerator.speed(), min_speed * sign_val, accel_per_sec, updates_per_sec);
    while ((mm > 0 && (lenc_delta * ENC_2_MM < mm ||
                       renc_delta * ENC_2_MM < mm)) ||
           ((mm < 0 && (lenc_delta * ENC_2_MM > mm ||
                        renc_delta * ENC_2_MM > mm))))
    {
        // both still have places to go
        if ((mm > 0 && lenc_delta * ENC_2_MM < mm && renc_delta * ENC_2_MM < mm) ||
            (mm < 0 && lenc_delta * ENC_2_MM > mm && renc_delta * ENC_2_MM > mm))
        {
            // if left wheel going faster, go slower
            if ((mm > 0 && lenc_delta > renc_delta) || (mm < 0 && lenc_delta < renc_delta))
            {
                create_drive_direct(static_cast<int>(decelerator.speed() * correction_proportion), static_cast<int>(decelerator.speed() / correction_proportion));
            }
            // if right wheel going faster, go slower
            else
            {
                create_drive_direct(static_cast<int>(decelerator.speed() / correction_proportion), static_cast<int>(decelerator.speed() * correction_proportion));
            }
        }
        // only lenc has places to go
        else if ((mm > 0 && lenc_delta * ENC_2_MM < mm) || (mm < 0 && lenc_delta * ENC_2_MM > mm))
        {
            create_drive_direct(min_speed * sign_val, sign_val);
        }
        // only renc has places to go
        else
        {
            create_drive_direct(sign_val, min_speed * sign_val);
        }

        // sleep
        decelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
    }
    create_drive_direct(0, 0);
}

void encoder_drive_straight_pid(int speed, double cm, double proportional_coefficient, double integral_coefficient, double derivative_coefficient, int min_speed, double accel_per_sec, int updates_per_second)
{
    create_drive_direct(0, 0);

    // initialize misc
    double cached_distance = 0;
    double goal_delta = 0, prev_goal_delta = 0;
    double dt = 1.0 / updates_per_second;
    const double mm = cm * 10;
    int sign_val = cm > 0 ? 1 : -1;
    size_t updates = 0;

    // initialize encoder variables
    int lenc_prev = 0, renc_prev = 0, lenc_delta = 0, renc_delta = 0;
    read_encoders(lenc_prev, renc_prev);

    LinearController accelerator(0, speed * sign_val, accel_per_sec, updates_per_second);
    PIDController l_controller(proportional_coefficient, integral_coefficient, derivative_coefficient, updates_per_second);
    PIDController r_controller(proportional_coefficient, integral_coefficient, derivative_coefficient, updates_per_second);
    ofstream out("/home/esuhsd/Documents/GitHub/ihsboost/out.txt");

    while ((mm > 0 && (lenc_delta * ENC_2_MM < mm / 2 &&
                       renc_delta * ENC_2_MM < mm / 2)) ||
           (mm < 0 && (lenc_delta * ENC_2_MM > mm / 2 &&
                       renc_delta * ENC_2_MM > mm / 2)))
    {
        // the desired goal val is the integral of the velocity
        // starting at velocity of 0, then going to velocity of v linearly -> forms a triangle
        // so we integrate using triangle integration
        // we also convert from mm to encoder values since speed is in mm
        goal_delta = (accelerator.speed() * updates * dt / 2.0) * MM_2_ENC;

        // update internals
        l_controller.step(lenc_delta, goal_delta);
        r_controller.step(renc_delta, goal_delta);

        // drive
        // we convert from encoder values to mm/sec
        // do enc_val * ENC_2_MM / dt
        double l_speed = -l_controller.speed() * ENC_2_MM / dt;
        double r_speed = -r_controller.speed() * ENC_2_MM / dt;
        out << "start; gd: " << goal_delta << ", l_delta:" << lenc_delta << ", l_speed: " << l_speed << ", r_delta: " << renc_delta << ", r_speed: " << r_speed << "l_raw: " << l_controller.speed() << ", r_raw: " << r_controller.speed() << endl;
        if (static_cast<int>(goal_delta * 10) != static_cast<int>(prev_goal_delta * 10))
        {
            l_speed = max(0.0, l_speed);
            r_speed = max(0.0, r_speed);
            out << "driving at " << l_speed << ", " << r_speed << endl;
            create_drive_direct(l_speed, r_speed);
            prev_goal_delta = goal_delta;
        }

        // sleep
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);

        if (accelerator.done())
        {
            out << "accelerator is done" << endl;
            cached_distance = (lenc_delta * ENC_2_MM + renc_delta * ENC_2_MM) / 2.0;
            break;
        }
        ++updates;
    }

    out << "cached distance is " << cached_distance << endl;
    // do any more driving until it is time to start decelerating
    updates = 0;
    double temp_goal_delta = goal_delta;
    while (cached_distance != 0 &&
           ((mm > 0 && (lenc_delta * ENC_2_MM < mm - cached_distance &&
                        renc_delta * ENC_2_MM < mm - cached_distance)) ||
            (mm < 0 && (lenc_delta * ENC_2_MM > mm - cached_distance &&
                        renc_delta * ENC_2_MM > mm - cached_distance))))
    {
        // at this point, we assume constant velocity, so rectangle integration
        goal_delta = temp_goal_delta + (accelerator.speed() * updates * dt) * MM_2_ENC;

        // update internals
        l_controller.step(lenc_delta, goal_delta);
        r_controller.step(renc_delta, goal_delta);

        // drive
        double l_speed = -l_controller.speed() * ENC_2_MM / dt;
        double r_speed = -r_controller.speed() * ENC_2_MM / dt;
        out << "middle; gd: " << goal_delta << ", l_delta:" << lenc_delta << ", l_speed: " << l_speed << ", r_delta: " << renc_delta << ", r_speed: " << r_speed << "l_raw: " << l_controller.speed() << ", r_raw: " << r_controller.speed() << endl;
        if (static_cast<int>(goal_delta * 10) != static_cast<int>(prev_goal_delta * 10))
        {
            l_speed = max(0.0, l_speed);
            r_speed = max(0.0, r_speed);
            out << "driving at " << l_speed << ", " << r_speed << endl;
            create_drive_direct(l_speed, r_speed);
            prev_goal_delta = goal_delta;
        }

        // sleep
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
        ++updates;
    }

    // start decelerating, go until both lenc and renc have reached the end
    temp_goal_delta = goal_delta;
    updates = 0;
    LinearController decelerator(accelerator.speed(), min_speed * sign_val, accel_per_sec, updates_per_second);
    while ((mm > 0 && (lenc_delta * ENC_2_MM < mm ||
                       renc_delta * ENC_2_MM < mm)) ||
           ((mm < 0 && (lenc_delta * ENC_2_MM > mm ||
                        renc_delta * ENC_2_MM > mm))))
    {
        // trapezoidal integration from accelerator.speed() to decelerator.speed() as b1 and b2
        // use (b1+b2)/2 * h, h is delta t
        goal_delta = temp_goal_delta + ((accelerator.speed() + decelerator.speed()) / 2.0 * updates * dt) * ENC_2_MM;

        // update internals
        l_controller.step(lenc_delta, goal_delta);
        r_controller.step(renc_delta, goal_delta);

        double l_speed = -l_controller.speed() * ENC_2_MM / dt;
        double r_speed = -r_controller.speed() * ENC_2_MM / dt;
        out << "end; gd: " << goal_delta << ", l_delta:" << lenc_delta << ", l_speed: " << l_speed << ", r_delta: " << renc_delta << ", r_speed: " << r_speed << "l_raw: " << l_controller.speed() << ", r_raw: " << r_controller.speed() << endl;
        if (static_cast<int>(goal_delta * 10) != static_cast<int>(prev_goal_delta * 10))
        {
            l_speed = max(0.0, l_speed);
            r_speed = max(0.0, r_speed);
            out << "driving at " << l_speed << ", " << r_speed << endl;
            create_drive_direct(l_speed, r_speed);
            prev_goal_delta = goal_delta;
        }

        // sleep
        decelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
        ++updates;
    }
    out << "should have gone " << lenc_delta * ENC_2_MM << "mm" << endl;
    out.close();
    create_drive_direct(0, 0);
}

void encoder_turn_degrees(int max_speed, int degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // this function uses the following formula:
    // angle in radians = (left wheel distance (mm) – right wheel distance (mm)) / wheel base distance (mm).

    // initialize misc
    double cached_angle_degrees = 0;
    double angle_degrees = 0;
    int left_sign_val = degrees > 0 ? 1 : -1;
    int right_sign_val = degrees > 0 ? -1 : 1;

    // initialize encoder variables
    int lenc_prev = 0, renc_prev = 0, lenc_delta = 0, renc_delta = 0;
    read_encoders(lenc_prev, renc_prev);

    LinearController accelerator(0, max_speed, accel_per_sec, updates_per_sec);

    while ((degrees > 0 && angle_degrees < degrees / 2) ||
           (degrees < 0 && angle_degrees > degrees / 2))
    {
        create_drive_direct(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
        angle_degrees = (lenc_delta * ENC_2_MM - renc_delta * ENC_2_MM) / (DIST_BETWEEN_WHEEL * 10) * rad2deg;

        if (accelerator.done())
        {
            cached_angle_degrees = angle_degrees;
            break;
        }
    }

    // do any more driving until it is time to start decelerating)
    while (cached_angle_degrees != 0 &&
           ((degrees > 0 && angle_degrees < degrees - cached_angle_degrees) ||
            (degrees < 0 && angle_degrees > degrees - cached_angle_degrees)))
    {
        create_drive_direct(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        msleep(accelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
        angle_degrees = (lenc_delta * ENC_2_MM - renc_delta * ENC_2_MM) / (DIST_BETWEEN_WHEEL * 10) * rad2deg;
    }

    // start decelerating, go until both lenc and renc have reached the end
    LinearController decelerator(accelerator.speed(), min_speed, accel_per_sec, updates_per_sec);
    while ((degrees > 0 && angle_degrees < degrees) ||
           (degrees < 0 && angle_degrees > degrees))

    {
        // both still have places to go
        create_drive_direct(static_cast<int>(decelerator.speed() * left_sign_val), static_cast<int>(decelerator.speed() * right_sign_val));

        // sleep
        decelerator.step();
        msleep(decelerator.get_msleep_time());

        // update encoders
        process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
        angle_degrees = (lenc_delta * ENC_2_MM - renc_delta * ENC_2_MM) / (DIST_BETWEEN_WHEEL * 10) * rad2deg;
    }
    create_drive_direct(0, 0);
}