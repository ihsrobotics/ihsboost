/**
 * @file imu_movement.hpp
 * @author Eliot Hall
 * @brief IMU (gyro/accel) based movement functions
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef IHSBOOST_GYRO_MOVEMENT_HPP
#define IHSBOOST_GYRO_MOVEMENT_HPP

#include "accumulator.hpp"
#include "config.hpp"
#include "speed.hpp"
#include <functional>
#include <memory>

/**
 * @brief Singleton class who's job it is to accumulate gyro values
 * @details Since this is a singleton class, use instance() to
 * get a pointer to the GyroSingleton object; this pointer can
 * be used to access the functionality of this class.
 * Note: `get_accumulator` is equivalent to getting
 * the current angle that the brain is at.
 *
 */
class GyroSingleton : public Accumulator
{
public:
    /**
     * @brief Get the current processed gyro reading, in gyro units
     *
     * @return double
     */
    static double get_gyro_val();

    /**
     * @brief Get the processed gyro reading, in degrees
     *
     * @return double
     */
    static double get_gyro_angle();

    /**
     * @brief Get the instance
     *
     * @return GyroSingleton*
     */
    static GyroSingleton *instance();

private:
    /**
     * @brief Construct a new Gyro Singleton object
     * @details this is marked private to enforce the singleton behavior
     *
     * @param updates_per_sec how many updates to do per sec by default
     */
    GyroSingleton(int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

    static std::shared_ptr<GyroSingleton> _instance; ///< the instance of GyroSingleton
};

/**
 * @brief Class that takes care of setting up the GyroSingleton for use
 * in client programs
 *
 */
class GyroSubscriber
{
public:
    /**
     * @brief Construct a new Gyro Subscriber object
     * @details this will get the GyroSingleton accumulating, record the starting
     * angle that the brain was at at this moment in time, and whether or not the
     * GyroSingleton was already accumulating when this subscriber was created.
     *
     * @param updates_per_sec how many times to read from the gyroscope per sec
     */
    GyroSubscriber(int updates_per_sec);
    /**
     * @brief Destroy the Gyro Subscriber object
     * @details if the GyroSingleton was already accumulating when this subscriber
     * was created, then the GyroSingleton will continue to accumulate. Else,
     * this will stop the accumulation.
     *
     */
    ~GyroSubscriber();

    /**
     * @brief Get the angle that the brain was at when this subscriber was created
     *
     * @return const double - the angle that the brain was at when the GyroSubscriber was created
     */
    const double &get_start_angle();

    /**
     * @brief Get the angle relative to when this GyroSubscriber was created.
     * @details For instance, a value of 90.35 means that the brain has
     * turned 90.35 degrees CW since this GyroSubscriber was created.
     *
     * @return double - the angle the brain has turned through, in degrees.
     */
    double get_relative_angle();

private:
    const double start_angle; ///< angle that the brain was at when this subscriber was created
    const double was_running; ///< whether or not the GyroSingleton was running when this subscriber was created
};

/**
 * @brief Drive the create straight using the gyroscope
 *
 * @param from_speed speed to start at
 * @param to_speed speed to cap at
 * @param stop_function a function that returns true when it is time to stop driving forward
 * @param correction_proportion how much the correction is by; defaults to .90,
 * should be between (0, 1)
 * @param accel_per_sec how fast the create will accelerate by, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_drive_straight(int from_speed, int to_speed, std::function<bool()> stop_function, double correction_proportion = get_config().getDouble("gyro_correction_proportion"), double accel_per_sec = get_config().getDouble("gyro_linear_accel"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

/**
 * @brief Turn the create a certain number of degrees using the gyroscope
 *
 * @param from_speed the speed to start at
 * @param to_speed the speed to end at
 * @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_turn_degrees(Speed from_speed, Speed to_speed, double degrees, double accel_per_sec = get_config().getDouble("gyro_linear_accel"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

/**
 * @brief Turns the create, starting at rest and ending at rest, and turning at max at max_speed
 *
 * @param max_speed the maximum speed to turn at, use a positive value
 * @param degrees how many degrees to turn (+ vals for CW, - vals for CCW)
 * @param min_speed the minimum speed to turn at, use a positive value, defaults to 11
 * @param accel_per_sec how fast to accelerate, defaults to 500
 * @param updates_per_sec how many updates the function will do per sec, defaults to 200
 */
void gyro_turn_degrees_v2(int max_speed, double degrees, int min_speed = get_config().getInt("gyro_turn_v2_min_speed"), double accel_per_sec = get_config().getDouble("gyro_linear_accel"), int updates_per_sec = get_config().getInt("gyro_updates_per_sec"));

#endif
/**@}*/