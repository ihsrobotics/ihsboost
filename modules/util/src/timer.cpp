#include "timer.hpp"

Timer::Timer(double time) : _time(static_cast<int>(time * 1000000)), _start_time(std::chrono::steady_clock::now()){};

bool Timer::operator()() const { return done(); }
bool Timer::done() const { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - _start_time).count() >= _time; }