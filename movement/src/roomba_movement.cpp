#include "roomba_movement.hpp"
#include "accelerator.hpp"
#include <kipr/wombat.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle)
{
    // calculate curl to get the time when angle is reached
    double time = abs(DIST_BETWEEN_WHEEL / (rightWheelSpeed - leftWheelSpeed) *
                      angle * deg2rad * 1000);

    create_drive_direct(leftWheelSpeed / LEFT_WHEEL_UNITS,
                        rightWheelSpeed / RIGHT_WHEEL_UNITS);
    msleep(time);
    create_drive_direct(0, 0);
}

void encoder_drive_straight(int max_speed, double cm, int min_speed, double correction_proportion, double accel_per_sec, int updates_per_sec)
{
    // initialize misc
    double cached_distance = 0;
    const double mm = cm * 10;
    int sign_val = cm > 0 ? 1 : -1;

    // initialize encoder variables
    long lenc_i = 0, renc_i = 0, lenc_f = 0, renc_f = 0, lenc_delta = 0, renc_delta = 0;
    _create_get_raw_encoders(&lenc_i, &renc_i);
    lenc_f = lenc_i;
    renc_f = renc_i;

    LinearAccelerator accelerator(0, max_speed * sign_val, accel_per_sec, updates_per_sec);

    while ((mm > 0 && (lenc_delta * (M_PI * 72.0 / 508.8) < mm / 2 &&
                       renc_delta * (M_PI * 72.0 / 508.8) < mm / 2)) ||
           (mm < 0 && (lenc_delta * (M_PI * 72.0 / 508.8) > mm / 2 &&
                       renc_delta * (M_PI * 72.0 / 508.8) > mm / 2)))
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
        _create_get_raw_encoders(&lenc_f, &renc_f);
        lenc_delta = lenc_f - lenc_i;
        renc_delta = renc_f - renc_i;

        if (accelerator.done())
        {
            cached_distance = (lenc_delta * (M_PI * 72.0 / 508.8) + renc_delta * (M_PI * 72.0 / 508.8)) / 2.0;
            break;
        }
    }

    // do any more driving until it is time to start decelerating
    while (cached_distance != 0 &&
           ((mm > 0 && (lenc_delta * (M_PI * 72.0 / 508.8) < mm - cached_distance &&
                        renc_delta * (M_PI * 72.0 / 508.8) < mm - cached_distance)) ||
            (mm < 0 && (lenc_delta * (M_PI * 72.0 / 508.8) > mm - cached_distance &&
                        renc_delta * (M_PI * 72.0 / 508.8) > mm - cached_distance))))
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
        _create_get_raw_encoders(&lenc_f, &renc_f);
        lenc_delta = lenc_f - lenc_i;
        renc_delta = renc_f - renc_i;
    }

    // start decelerating, go until both lenc and renc have reached the end
    LinearAccelerator decelerator(accelerator.speed(), min_speed * sign_val, accel_per_sec, updates_per_sec);
    while ((mm > 0 && (lenc_delta * (M_PI * 72.0 / 508.8) < mm ||
                       renc_delta * (M_PI * 72.0 / 508.8) < mm)) ||
           ((mm < 0 && (lenc_delta * (M_PI * 72.0 / 508.8) > mm ||
                        renc_delta * (M_PI * 72.0 / 508.8) > mm))))
    {
        // both still have places to go
        if ((mm > 0 && lenc_delta * (M_PI * 72.0 / 508.8) < mm && renc_delta * (M_PI * 72.0 / 508.8) < mm) ||
            (mm < 0 && lenc_delta * (M_PI * 72.0 / 508.8) > mm && renc_delta * (M_PI * 72.0 / 508.8) > mm))
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
        else if ((mm > 0 && lenc_delta * (M_PI * 72.0 / 508.8) < mm) || (mm < 0 && lenc_delta * (M_PI * 72.0 / 508.8) > mm))
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
        _create_get_raw_encoders(&lenc_f, &renc_f);
        lenc_delta = lenc_f - lenc_i;
        renc_delta = renc_f - renc_i;
    }
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
    long lenc_i = 0, renc_i = 0, lenc_f = 0, renc_f = 0, lenc_delta = 0, renc_delta = 0;
    _create_get_raw_encoders(&lenc_i, &renc_i);
    lenc_f = lenc_i;
    renc_f = renc_i;

    LinearAccelerator accelerator(0, max_speed, accel_per_sec, updates_per_sec);

    while ((degrees > 0 && angle_degrees < degrees / 2) ||
           (degrees < 0 && angle_degrees > degrees / 2))
    {
        create_drive_direct(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        _create_get_raw_encoders(&lenc_f, &renc_f);
        lenc_delta = lenc_f - lenc_i;
        renc_delta = renc_f - renc_i;
        angle_degrees = (lenc_delta * (M_PI * 72.0 / 508.8) - renc_delta * (M_PI * 72.0 / 508.8)) / (DIST_BETWEEN_WHEEL * 10) * rad2deg;
        cout << "lenc delta is " << lenc_delta << " and renc delta is " << renc_delta << " and angle_degrees " << angle_degrees << endl;

        if (accelerator.done())
        {
            cached_angle_degrees = angle_degrees;
            break;
        }
    }

    // do any more driving until it is time to start decelerating
    while (cached_angle_degrees != 0 &&
           ((degrees > 0 && angle_degrees < degrees - cached_angle_degrees) ||
            (degrees < 0 && angle_degrees > degrees - cached_angle_degrees)))
    {
        create_drive_direct(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        msleep(accelerator.get_msleep_time());

        // update encoders
        _create_get_raw_encoders(&lenc_f, &renc_f);
        lenc_delta = lenc_f - lenc_i;
        renc_delta = renc_f - renc_i;
        angle_degrees = (lenc_delta * (M_PI * 72.0 / 508.8) - renc_delta * (M_PI * 72.0 / 508.8)) / (DIST_BETWEEN_WHEEL * 10) * rad2deg;
        cout << "lenc delta is " << lenc_delta << " and renc delta is " << renc_delta << " and angle_degrees " << angle_degrees << endl;
    }

    // start decelerating, go until both lenc and renc have reached the end
    LinearAccelerator decelerator(accelerator.speed(), min_speed, accel_per_sec, updates_per_sec);
    while ((degrees > 0 && angle_degrees < degrees) ||
           (degrees < 0 && angle_degrees > degrees))

    {
        // both still have places to go
        create_drive_direct(static_cast<int>(accelerator.speed() * left_sign_val), static_cast<int>(accelerator.speed() * right_sign_val));

        // sleep
        decelerator.step();
        msleep(accelerator.get_msleep_time());

        // update encoders
        _create_get_raw_encoders(&lenc_f, &renc_f);
        lenc_delta = lenc_f - lenc_i;
        renc_delta = renc_f - renc_i;
        angle_degrees = (lenc_delta * (M_PI * 72.0 / 508.8) - renc_delta * (M_PI * 72.0 / 508.8)) / (DIST_BETWEEN_WHEEL * 10) * rad2deg;
        cout << "lenc delta is " << lenc_delta << " and renc delta is " << renc_delta << " and angle_degrees " << angle_degrees << endl;
    }
    create_drive_direct(0, 0);
}