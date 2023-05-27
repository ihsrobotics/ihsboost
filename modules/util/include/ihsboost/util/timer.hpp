/**
 * @file timer.hpp
 * @author Eliot Hall
 * @brief Timer class definition
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef IHSBOOST_TIMER_HPP
#define IHSBOOST_TIMER_HPP

#include <chrono>

/**
 * @brief A class that implements a timer, useful when you want
 * to line-follow for a certain amount of time.
 *
 */
class Timer {
  public:
    /**
     * @brief Construct a new Timer object
     *
     * @param time how long to set the timer for, in seconds
     */
    Timer(double time);

    /**
     * @brief Returns false until it is time to stop
     * (when it has been `time` seconds)
     *
     * @return true - once it has been `time` seconds
     * @return false - if it hasn't been `time` seconds yet
     */
    bool operator()() const;

    /**
     * @brief Returns false until it is time to stop
     * (when it has been `time` seconds)
     *
     * @return true - once it has been `time` seconds
     * @return false - if it hasn't been `time` seconds yet
     */
    bool done() const;

  private:
    int _time;
    std::chrono::steady_clock::time_point _start_time;
};

#endif
/**@}*/