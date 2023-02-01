#ifndef ACCUMULATOR_HPP
#define ACCUMULATOR_HPP

#include "threading.hpp"
#include <functional>
#include <kipr/wombat.h>

class Accumulator
{
public:
    Accumulator(std::function<double()> callable, int updates_per_sec)
        : callable(callable), flag(true), multiplier(1 / static_cast<double>(updates_per_sec)),
          msleep_time(1000 / updates_per_sec){};

    /**
     * @brief Start accumulating values
     *
     */
    void start_accumulating();

    /**
     * @brief Stop accumulating values
     *
     */
    void stop_accumulating();

    /**
     * @brief Return the current value of the accumulator
     *
     * @return const volatile&
     */
    const volatile double &get_accumulator() { return accumulator; }

private:
    static void accumulate(Accumulator *a);

    Threadable<void(Accumulator *a), Accumulator *> *t;
    int msleep_time;
    double multiplier;
    std::function<double()> callable;
    volatile double accumulator;
    volatile bool flag;
};

#endif