/**
 * @file accumulator.hpp
 * @author Eliot Hall
 * @brief Accumulator class for integrating values
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup threading_id
 * @{
 */
#ifndef IHSBOOST_ACCUMULATOR_HPP
#define IHSBOOST_ACCUMULATOR_HPP

#include "background_task.hpp"
#include <functional>

/**
 * @brief Integrate values returned by the given function
 *
 */
class Accumulator : public BackgroundTask {
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
    virtual ~Accumulator();

    /**
     * @brief Change how many updates per second this accumulator does
     *
     * @param updates_per_sec how many updates to do per second
     */
    virtual void set_updates_per_sec(int updates_per_sec);

    /**
     * @brief Return the current value of the accumulator
     * @details the value returned is \f$\int f(t)dt\f$
     *
     * @return const volatile& - the current value
     */
    const volatile double &get_accumulator();

  protected:
    /**
     * @brief Function stub that BackgroundTask will
     * call
     *
     */
    virtual void function();

  private:
    std::function<double()> callable;
    volatile double accumulator;

    double multiplier;
};

#endif
/**@}*/