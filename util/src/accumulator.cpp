#include "accumulator.hpp"

Accumulator::Accumulator(std::function<double()> callable, int updates_per_sec)
    : callable(callable), t(nullptr), accumulator(0), flag(true), msleep_time(1000 / updates_per_sec),
      multiplier(1 / static_cast<double>(updates_per_sec)){};

Accumulator::~Accumulator()
{
    stop_accumulating();
}

void Accumulator::start_accumulating()
{
    if (t == nullptr)
    {
        // accumulate values in separate thread
        t = new Threadable<void(Accumulator * a), Accumulator *>(accumulate, this);
    }
}

void Accumulator::stop_accumulating()
{
    flag = false;

    // only call it if t isn't a nullptr to prevent segmentation faults
    if (t != nullptr)
    {
        // wait till finished
        while (!(*t)())
            ;

        // cleanup
        delete t;
        t = nullptr;
    }
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