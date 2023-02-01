#include "accumulator.hpp"

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