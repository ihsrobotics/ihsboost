#ifndef TIMER_HPP
#define TIMER_HPP

/**
 * @brief A class that implements a timer, useful when you want
 * to line-follow for a certain amount of time.
 *
 */
class Timer
{
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
    bool operator()();

private:
    double _time;
    double _start_time;
};

#endif