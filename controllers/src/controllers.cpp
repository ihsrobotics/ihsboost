#include <stdlib.h>
#include <math.h>
#include "controllers.hpp"

const int AccelerateController::get_msleep_time() const { return 1000 / _updates_per_sec; }

const double AccelerateController::speed() const { return _cur_speed; }

LinearController::LinearController(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec) : AccelerateController(from_speed, to_speed, accel_per_sec, updates_per_sec)
{
    sign = from_speed < to_speed ? 1 : -1;
}
void LinearController::step()
{
    if (done())
    {
        return;
    }
    _cur_speed += sign * _accel_per_sec / _updates_per_sec;
    ++_num_steps;
}
bool LinearController::done()
{
    return (sign == 1 && _cur_speed >= _to_speed) || (sign == -1 && _cur_speed <= _to_speed);
}

SinusoidalController::SinusoidalController(int from_speed, int to_speed, double avg_accel_per_sec, int updates_per_sec) : AccelerateController(from_speed, to_speed, avg_accel_per_sec, updates_per_sec)
{
    // store our delta speed
    delta_speed = to_speed - from_speed;

    // calculate necessary updates
    // delta * updates/sec / delta/sec = delta * updates / delta = updates
    necessary_updates = abs(delta_speed * updates_per_sec / avg_accel_per_sec);
};
void SinusoidalController::step()
{
    if (done())
    {
        return;
    }
    ++_num_steps; // increment num steps first since we use it to calculate our sin val
    _cur_speed = _from_speed + delta_speed * sin(_num_steps / necessary_updates * M_PI / 2);
}
bool SinusoidalController::done()
{
    return _num_steps >= necessary_updates;
}

PIDController::PIDController(double Kp, double Ki, double Kd, int updates_per_second) : Kp(Kp), Ki(Ki), Kd(Kd), error_integral(0), past_error(0), dt(1.0 / updates_per_second), cur_speed(0){};

void PIDController::step(double error)
{
    // trapezoidal integration, (b1+b2)/2 * h
    error_integral += (past_error + error) * .5 * dt;

    // derivative of error with respect to time
    // can be said to be delta error / delta t
    cur_speed = Kp * error + Ki * error_integral + Kd * (error - past_error) / dt;

    // update past error
    past_error = error;
}

void PIDController::step(double value, double goal)
{
    step(value - goal);
}

const double PIDController::speed() const
{
    return cur_speed;
}