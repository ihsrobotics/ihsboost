#include "accumulator.hpp"

Accumulator::Accumulator(std::function<double()> callable, int updates_per_sec)
    : callable(callable), flag(true), multiplier(1 / static_cast<double>(updates_per_sec)),
      msleep_time(1000 / updates_per_sec){};

void Accumulator::start_accumulating()
{
    // accumulate values in separate thread
    t = new Threadable(accumulate, this);
}

void Accumulator::stop_accumulating()
{
    flag = false;

    // wait till finished
    while (!(*t)())
        ;

    // cleanup
    delete t;
}

void Accumulator::accumulate(Accumulator *a)
{
    while (a->flag)
    {
        a->accumulator += a->callable() * a->multiplier;
        msleep(a->msleep_time);
    }
}

const volatile double &Accumulator::get_accumulator() { return accumulator; }