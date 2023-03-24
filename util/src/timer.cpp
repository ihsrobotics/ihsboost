#include "timer.hpp"
#include <kipr/wombat.h>

Timer::Timer(double time) : _time(time), _start_time(seconds()){};

bool Timer::operator()() const { return done(); }
bool Timer::done() const { return seconds() - _start_time >= _time; }