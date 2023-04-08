/**
 * @file maneuvers.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef IHSBOOST_MANEUVERS_HPP
#define IHSBOOST_MANEUVERS_HPP

#include "singletons.hpp"
#include "speed.hpp"

void turn_degrees(Subscriber *subscriber, int max_speed, double degrees, int min_speed, double accel_per_sec, int updates_per_sec);

void turn_degrees(Subscriber *subscriber, Speed turn_speed, double degrees, int updates_per_sec);

void drive_straight(Subscriber *subscriber, int speed, std::function<bool()> condition, bool stop, double correction_proportion, int updates_per_sec);

#endif