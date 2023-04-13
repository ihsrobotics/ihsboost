#include "roomba_movement.hpp"
#include "ihsboost/controllers/controllers.hpp"
#include "movement_constants.hpp"
#include "maneuvers.hpp"
#include <kipr/wombat.h>
#include <algorithm>
#include <limits>
using std::abs;
using std::min;

// --------------------------------------------- ENCODER FUNCTIONS ---------------------------------------------
// Functions in this section deal with the encoder functions
// --------------------------------------------------------------------------------------------------------

void rotate(double leftWheelSpeed, double rightWheelSpeed, double angle, double left_wheel_units, double right_wheel_units)
{
    // calculate curl to get the time when angle is reached
    double time = abs(DIST_BETWEEN_WHEEL / (rightWheelSpeed - leftWheelSpeed) *
                      angle * deg2rad_mult * 1000.0);

    create_drive_direct(static_cast<int>(leftWheelSpeed / left_wheel_units),
                        static_cast<int>(rightWheelSpeed / right_wheel_units));
    msleep(static_cast<int>(time));
    create_drive_direct(0, 0);
}

double encoder_drive_straight(int speed, std::function<bool()> condition, bool stop, double correction_proportion, int updates_per_sec)
{
    // initialize encoder variables
    EncoderSubscriber subscriber(updates_per_sec);

    // drive straight using the subscriber
    drive_straight(&subscriber, speed, condition, stop, correction_proportion, updates_per_sec);

    // return distance traveled
    return (subscriber.get_relative_lenc_delta() * ENC_2_MM + subscriber.get_relative_renc_delta() * ENC_2_MM) / 2.0;
}

double encoder_drive_straight(int max_speed, double cm, bool stop, int min_speed, double correction_proportion, double accel_per_sec, int updates_per_sec)
{
    // initialize misc
    const double mm = cm * 10;
    int sign_val = cm > 0 ? 1 : -1;
    // initialize encoder variables
    EncoderSubscriber subscriber(updates_per_sec);

    THREE_PART_LOOP(
        max_speed, min_speed, accel_per_sec, updates_per_sec, if ((mm > 0 && subscriber.get_relative_lenc_delta() > subscriber.get_relative_renc_delta()) || (mm < 0 && subscriber.get_relative_lenc_delta() < subscriber.get_relative_renc_delta())) { create_drive_direct(static_cast<int>(accelerator.speed() * correction_proportion), static_cast<int>(accelerator.speed() / correction_proportion)); } else {
            create_drive_direct(static_cast<int>(accelerator.speed() / correction_proportion), static_cast<int>(accelerator.speed() * correction_proportion));
        },
        if ((mm > 0 && subscriber.get_relative_lenc_delta() * ENC_2_MM < mm && subscriber.get_relative_renc_delta() * ENC_2_MM < mm) ||
            (mm < 0 && subscriber.get_relative_lenc_delta() * ENC_2_MM > mm && subscriber.get_relative_renc_delta() * ENC_2_MM > mm)) {
            if ((mm > 0 && subscriber.get_relative_lenc_delta() > subscriber.get_relative_renc_delta()) || (mm < 0 && subscriber.get_relative_lenc_delta() < subscriber.get_relative_renc_delta()))
            {
                create_drive_direct(static_cast<int>(decelerator.speed() * correction_proportion), static_cast<int>(decelerator.speed() / correction_proportion));
            }
            else
            {
                create_drive_direct(static_cast<int>(decelerator.speed() / correction_proportion), static_cast<int>(decelerator.speed() * correction_proportion));
            }
        } else if ((mm > 0 && subscriber.get_relative_lenc_delta() * ENC_2_MM < mm) || (mm < 0 && subscriber.get_relative_lenc_delta() * ENC_2_MM > mm)) {
            create_drive_direct(min_speed * sign_val, sign_val);
        } else {
            create_drive_direct(sign_val, min_speed * sign_val);
        },
        mm > 0 && subscriber.get_relative_distance() < mm, mm < 0 && subscriber.get_relative_distance() > mm, subscriber.get_relative_distance())

    if (stop)
    {
        create_drive_direct(0, 0);
    }
    else
    {
        create_drive_direct(min_speed, min_speed);
    }
    return (subscriber.get_relative_lenc_delta() * ENC_2_MM + subscriber.get_relative_renc_delta() * ENC_2_MM) / 2.0;
}

double process_speed(double correction_val, double regular_val, int min_speed)
{
    if (static_cast<int>(correction_val * 10) == 0)
    {
        return regular_val;
    }
    if (abs(correction_val + regular_val) < min_speed)
    {
        return min_speed * (correction_val > 0 ? 1 : -1);
    }
    return correction_val + regular_val;
}

void encoder_drive_straight_pid(int speed, double cm, double proportional_coefficient, double integral_coefficient, double derivative_coefficient, bool stop, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // so apparently create_drive_direct is rly in encoders/sec, not mm/sec
    create_drive_direct(0, 0);

    // initialize misc
    double cached_distance = 0;
    double goal_delta = 0;
    double dt = 10.0 / updates_per_sec;
    const double mm = cm * 10;
    int sign_val = cm > 0 ? 1 : -1;
    size_t updates = 0;

    // initialize encoder variables
    EncoderSubscriber subscriber(updates_per_sec);
    int lenc_delta_prev = 0;
    int renc_delta_prev = 0;

    LinearController accelerator(0, speed * sign_val, accel_per_sec, updates_per_sec);
    PIDController l_controller(proportional_coefficient, integral_coefficient, derivative_coefficient, updates_per_sec);
    PIDController r_controller(proportional_coefficient, integral_coefficient, derivative_coefficient, updates_per_sec);

    while ((mm > 0 && goal_delta < mm * MM_2_ENC / 2) ||
           (mm < 0 && goal_delta > mm * MM_2_ENC / 2))
    {
        // the desired goal val is the integral of the velocity
        // starting at velocity of 0, then going to velocity of v linearly -> forms a triangle
        // so we integrate using triangle integration
        goal_delta = accelerator.speed() * static_cast<double>(updates) * dt / 2.0;

        if (subscriber.get_relative_lenc_delta() != lenc_delta_prev || subscriber.get_relative_renc_delta() != renc_delta_prev)
        {
            // update internals
            l_controller.step(subscriber.get_relative_lenc_delta(), goal_delta);
            r_controller.step(subscriber.get_relative_renc_delta(), goal_delta);

            // drive
            // we convert from encoder values to mm/sec
            // do enc_val * ENC_2_MM / dt
            double l_speed = -l_controller.speed() * ENC_2_MM / dt;
            double r_speed = -r_controller.speed() * ENC_2_MM / dt;

            create_drive_direct(static_cast<int>(process_speed(l_speed, accelerator.speed(), min_speed)), static_cast<int>(process_speed(r_speed, accelerator.speed(), min_speed)));
            lenc_delta_prev = subscriber.get_relative_lenc_delta();
            renc_delta_prev = subscriber.get_relative_renc_delta();
        }

        // sleep
        accelerator.step();
        msleep(accelerator.get_msleep_time());

        if (accelerator.done())
        {
            cached_distance = goal_delta * ENC_2_MM;
            break;
        }
        ++updates;
    }

    // do any more driving until it is time to start decelerating
    updates = 0;
    double temp_goal_delta = goal_delta;
    while (cached_distance != 0 &&
           ((mm > 0 && goal_delta < (mm - cached_distance) * MM_2_ENC) ||
            (mm < 0 && goal_delta > (mm - cached_distance) * MM_2_ENC)))
    {
        // at this point, we assume constant velocity, so rectangle integration
        goal_delta = min(temp_goal_delta + (accelerator.speed() * static_cast<double>(updates) * dt), (mm - cached_distance) * MM_2_ENC);

        if (subscriber.get_relative_lenc_delta() != lenc_delta_prev || subscriber.get_relative_renc_delta() != renc_delta_prev)
        {
            // update internals
            l_controller.step(subscriber.get_relative_lenc_delta(), goal_delta);
            r_controller.step(subscriber.get_relative_renc_delta(), goal_delta);

            // drive
            // we convert from encoder values to mm/sec
            // do enc_val * ENC_2_MM / dt
            double l_speed = -l_controller.speed() * ENC_2_MM / dt;
            double r_speed = -r_controller.speed() * ENC_2_MM / dt;

            create_drive_direct(static_cast<int>(process_speed(l_speed, accelerator.speed(), min_speed)), static_cast<int>(process_speed(r_speed, accelerator.speed(), min_speed)));
            lenc_delta_prev = subscriber.get_relative_lenc_delta();
            renc_delta_prev = subscriber.get_relative_renc_delta();
        }

        // sleep
        msleep(accelerator.get_msleep_time());

        ++updates;
    }

    // start decelerating, go until both lenc and renc have reached the end
    goal_delta = (mm - cached_distance) * MM_2_ENC;
    temp_goal_delta = goal_delta;
    updates = 0;
    LinearController decelerator(static_cast<int>(accelerator.speed()), min_speed * sign_val, accel_per_sec, updates_per_sec);
    while ((mm > 0 && (subscriber.get_relative_lenc_delta() < mm * MM_2_ENC ||
                       subscriber.get_relative_renc_delta() < mm * MM_2_ENC)) ||
           ((mm < 0 && (subscriber.get_relative_lenc_delta() > mm * MM_2_ENC ||
                        subscriber.get_relative_renc_delta() > mm * MM_2_ENC))))
    {
        // trapezoidal integration from accelerator.speed() to decelerator.speed() as b1 and b2
        // use (b1+b2)/2 * h, h is delta t
        goal_delta = min(temp_goal_delta + ((accelerator.speed() + decelerator.speed()) / 2.0 * static_cast<double>(updates) * dt), mm * MM_2_ENC);

        if (subscriber.get_relative_lenc_delta() != lenc_delta_prev || subscriber.get_relative_renc_delta() != renc_delta_prev)
        {
            // update internals
            l_controller.step(subscriber.get_relative_lenc_delta(), goal_delta);
            r_controller.step(subscriber.get_relative_renc_delta(), goal_delta);

            // drive
            // we convert from encoder values to mm/sec
            // do enc_val * ENC_2_MM / dt
            double l_speed = -l_controller.speed() * ENC_2_MM / dt;
            double r_speed = -r_controller.speed() * ENC_2_MM / dt;

            create_drive_direct(static_cast<int>(process_speed(l_speed, decelerator.speed(), min_speed)), static_cast<int>(process_speed(r_speed, decelerator.speed(), min_speed)));
            lenc_delta_prev = subscriber.get_relative_lenc_delta();
            renc_delta_prev = subscriber.get_relative_renc_delta();
        }

        // sleep
        decelerator.step();
        msleep(accelerator.get_msleep_time());

        ++updates;
    }
    if (stop)
    {
        create_drive_direct(0, 0);
    }
    else
    {
        create_drive_direct(min_speed, min_speed);
    }
}

void encoder_turn_degrees(int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec)
{
    // this function uses the following formula:

    // initialize encoder variables
    EncoderSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, max_speed, degrees, min_speed, accel_per_sec, updates_per_sec);
}

void encoder_turn_degrees(Speed turn_speed, double degrees, int updates_per_sec)
{
    // initialize encoders
    EncoderSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, turn_speed, degrees, updates_per_sec);
}