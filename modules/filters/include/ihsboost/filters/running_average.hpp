/**
 * @file running_average.hpp
 * @author Eliot Hall
 * @brief Running Average class
 * @version 0.1
 * @date 2023-04-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup filters_id
 * @{
 */
#ifndef IHSBOOST_RUNNING_AVERAGE_HPP
#define IHSBOOST_RUNNING_AVERAGE_HPP

#include "filter.hpp"
#include <cstdint>
#include <numeric>

/**
 * @brief Running Average filter that takes the average of
 * `size` readings
 *
 * @tparam FilterType the type of the values that the filter will filter
 * @tparam size the number of readings to average
 */
template <typename FilterType, std::size_t size>
class RunningAverage : public Filter<FilterType>
{
public:
    /**
     * @brief Construct a new Running Average object
     *
     */
    RunningAverage() : Filter<FilterType>(), buf(), complete(false), idx(0){};

    /**
     * @brief Update the filter with the given value and return the filtered
     * value (the current average)
     *
     * @param val the value to filter
     * @return double the current running average
     */
    virtual double filter(FilterType val)
    {
        buf[idx] = val;

        // update idx and complete flags
        ++idx;
        if (!complete && idx == size)
        {
            complete = true;
        }
        idx %= size;

        // calculate and return average
        if (complete)
        {
            double total = std::accumulate(buf, buf + size, 0);
            return total / size;
        }
        else
        {
            double total = std::accumulate(buf, buf + idx);
            return total / idx;
        }
    }

private:
    FilterType buf[size];
    bool complete;
    size_t idx;
};

#endif
/**@}*/