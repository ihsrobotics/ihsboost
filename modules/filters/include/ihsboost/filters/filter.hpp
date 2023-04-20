/**
 * @file filter.hpp
 * @author Eliot Hall
 * @brief Abstract filter class
 * @version 0.1
 * @date 2023-04-14
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IHSBOOST_FILTER_HPP
#define IHSBOOST_FILTER_HPP

/**
 * @brief Abstract class representing a value filter
 *
 * @tparam FilterType the type to filter
 */
template <typename FilterType>
class Filter
{
public:
    /**
     * @brief Construct a new Filter object
     *
     */
    Filter() = default;

    /**
     * @brief Destroy the Filter object
     *
     */
    virtual ~Filter() = default;

    /**
     * @brief Update the filter with the given reading and return
     * the updated filtered value
     *
     * @param val the value to filter
     * @return double the current filtered value
     */
    virtual double filter(FilterType val) = 0;
};

#endif