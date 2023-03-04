/**
 * @file accumulator.hpp
 * @author Eliot Hall
 * @brief Accumulator class for integrating values
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef ACCUMULATOR_HPP
#define ACCUMULATOR_HPP

#include "threading.hpp"
#include <functional>
#include <kipr/wombat.h>

/**
 * @brief Integrate values returned by the given function
 *
 */
class Accumulator
{
public:
    /**
     * @brief Construct a new Accumulator object
     * @details this will integrate values returned by the callable,
     * meaning that the current value will be the integral of the function
     * d time.
     *
     * @param callable the function to accumulate values from
     * @param updates_per_sec how many times per second to accumulate
     */
    Accumulator(std::function<double()> callable, int updates_per_sec);

    /**
     * @brief Destroy the Accumulator object
     * @details cleans up any accumulating that is currently being done
     */
    ~Accumulator();

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
     * @details the value returned is \f$\int f(t)dt\f$
     *
     * @return const volatile&
     */
    const volatile double &get_accumulator();

private:
    static void accumulate(Accumulator *a);

    std::function<double()> callable;
    Threadable<void(Accumulator *a), Accumulator *> *t;
    volatile double accumulator;
    volatile bool flag;

    int msleep_time;
    double multiplier;
};

#endif
/**@}*/