/**
 * @file maneuvers.hpp
 * @author Eliot Hall
 * @brief Abstract maneuvers
 * @version 0.1
 * @date 2023-04-07
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef IHSBOOST_MANEUVERS_HPP
#define IHSBOOST_MANEUVERS_HPP

#include "singletons.hpp"
#include "ihsboost/util/speed.hpp"

/**
 * @brief Turn the given number of degrees using the given subscriber
 *
 * @param subscriber the subscriber that provides information on the current angle
 * @param max_speed the maximum speed to turn at, should be positive
 * @param degrees the number of degrees to turn (+ vals for CW, - vals for CCW)
 * @param min_speed the minimum speed to turn at, should be positive
 * @param accel_per_sec how fast to accelerate
 * @param updates_per_sec how many updates to do per second
 */
void turn_degrees(Subscriber *subscriber, int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec);

/**
 * @brief Turn the given number of degrees sat the given speed using the given subscriber
 *
 * @param subscriber the subscriber that provides information on the current angle
 * @param turn_speed the speed to turn at
 * @param degrees the number of degrees to turn (+ vals for CW, - vals for CCW)
 * @param updates_per_sec how many updates to do per second
 */
void turn_degrees(Subscriber *subscriber, Speed turn_speed, double degrees, int updates_per_sec);

/**
 * @brief Drive straight using the given subscriber until told to stop
 *
 * @param subscriber the subscriber that provides information on the current angle
 * @param speed the speed to drive at, can be positive or negative
 * @param condition the stop condition; it should return true when it is time to stop
 * @param stop whether or not to stop the motors after finishing driving; if it is false, the motors will continue spinning, if it is true, they will not
 * continue spinning
 * @param correction_proportion how much to correct by; values closer to 1 mean less correction, values closer to 0 mean more correction.
 * @param updates_per_sec how many updates to do per second
 */
void drive_straight(Subscriber *subscriber, int speed, std::function<bool()> condition, bool stop, double correction_proportion, int updates_per_sec);

#endif
/**@}*/