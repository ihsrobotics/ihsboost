/**
 * @file speed.hpp
 * @author Eliot Hall
 * @brief Speed struct for representing speeds
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup util_id
 * @{
 */
#ifndef IHSBOOST_SPEED_HPP
#define IHSBOOST_SPEED_HPP

/**
 * @brief An object to represent speed in terms of left and right wheel speeds
 *
 */
struct Speed
{
    /**
     * @brief Construct a new Speed object
     *
     * @param left_speed the speed of the left wheel
     * @param right_speed the speed of the right wheel
     */
    Speed(int left_speed, int right_speed);

    /**
     * @brief Return a new Speed object representing the difference of two speeds
     * @details equivalent to constructing a new Speed object with
     * speeds left - o.left and right - o.right, respectively
     *
     * @param o the other speed
     * @return Speed
     */
    Speed operator-(const Speed o) const;

    /**
     * @brief Return a new Speed object representing the sum of two speeds
     * @details equivalent to constructing a new Speed object with
     * speeds left + o.left and right + o.right, respectively
     *
     * @param o the other speed
     * @return Speed
     */
    Speed operator+(const Speed o) const;

    /**
     * @brief Adds the other speed to this one
     *
     * @param o the other speed
     * @return Speed
     */
    Speed &operator+=(const Speed o);

    /**
     * @brief Subtracts the other speed from this one
     *
     * @param o the other speed
     * @return Speed
     */
    Speed &operator-=(const Speed o);

    /**
     * @brief Check if the other speed is equal by comparing left and right speeds
     *
     * @param o the speed to check equality with
     * @return true
     * @return false
     */
    bool operator==(const Speed o) const;

    /**
     * @brief Check if the other speed is not equal by comparing left and right speeds
     *
     * @param o the speed to check inequality with
     * @return true
     * @return false
     */
    bool operator!=(const Speed o) const;

    int left;  ///< the speed of the left wheel
    int right; ///< the speed of the right wheel
};

#endif
/**@}*/