#include "accumulator.hpp"
#include <kipr/wombat.h>

Accumulator::Accumulator(std::function<double()> callable, int updates_per_sec)
    : BackgroundTask(updates_per_sec), callable(callable),
      accumulator(0), multiplier(1 / static_cast<double>(updates_per_sec)){};

Accumulator::~Accumulator() {}

void Accumulator::function()
{
    accumulator += callable() * multiplier;
}

void Accumulator::set_updates_per_sec(int updates_per_sec)
{
    BackgroundTask::set_updates_per_sec(updates_per_sec);
    multiplier = 1 / static_cast<double>(updates_per_sec);
}

const volatile double &Accumulator::get_accumulator() { return accumulator; }