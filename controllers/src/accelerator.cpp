#include <stdlib.h>
#include <math.h>
#include "accelerator.hpp"

LinearAccelerator::LinearAccelerator(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec) : AccelerateController(from_speed, to_speed, accel_per_sec, updates_per_sec)
{
    sign = from_speed < to_speed ? 1 : -1;
}
void LinearAccelerator::step()
{
    if (done())
    {
        return;
    }
    _cur_speed += sign * _accel_per_sec / _updates_per_sec;
    ++_num_steps;
}
bool LinearAccelerator::done()
{
    return (sign == 1 && _cur_speed >= _to_speed) || (sign == -1 && _cur_speed <= _to_speed);
}

SinusoidalAccelerator::SinusoidalAccelerator(int from_speed, int to_speed, double avg_accel_per_sec, int updates_per_sec) : AccelerateController(from_speed, to_speed, avg_accel_per_sec, updates_per_sec)
{
    delta_speed = to_speed - from_speed;
    necessary_updates = abs(delta_speed * updates_per_sec / avg_accel_per_sec);
};
void SinusoidalAccelerator::step()
{
    if (done())
    {
        return;
    }
    sin_val = sin(_num_steps / necessary_updates * M_PI / 2);
    _cur_speed = _from_speed + delta_speed * sin_val;
    ++_num_steps;
}
bool SinusoidalAccelerator::done()
{
    return _num_steps >= necessary_updates;
}
