/**
 * @file singletons.hpp
 * @author Eliot Hall
 * @brief Singletons and subscribers
 * @version 0.1
 * @date 2023-04-07
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup movement_id
 * @{
 */
#ifndef IHSBOOST_SINGLETONS_HPP
#define IHSBOOST_SINGLETONS_HPP

#include <memory>
#include "ihsboost/threading/background_task.hpp"
#include "ihsboost/threading/accumulator.hpp"
#include "ihsboost/util/config.hpp"

/**
 * @brief Abstract class representing a subscriber that
 * can return information regarding the current angle
 *
 */
class AngleSubscriber
{
public:
    virtual double get_relative_angle() = 0;
};

/**
 * @brief Abstract class representing a subscriber that
 * can return information regarding the current distance
 * traveled
 *
 */
class DistanceSubscriber
{
public:
    virtual double get_relative_distance() = 0;
};

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
     * @return GyroSingleton* - a pointer to the GyroSingleton object
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
 * @brief Singleton class who's job it is to read encoder values
 * @details Since this is a singleton class, use instance() to
 * get a pointer to the EncoderSingleton object; this pointer can
 * be used to access the functionality of this class.
 *
 */
class EncoderSingleton : public BackgroundTask
{
public:
    /**
     * @brief Get the instance
     *
     * @return EncoderSingleton* - a pointer to the EncoderSingleton object
     */
    static EncoderSingleton *instance();

    /**
     * @brief Read create encoders into lenc and renc
     * @details This gives raw encoder values, which range from [-32768, 32767]
     *
     * @param[out] lenc the int that will store the left encoder value
     * @param[out] renc the int that will store the right encoder value
     */
    static void read_encoders(int &lenc, int &renc);

    /**
     * @brief Process encoders so that, even if overflow/underflow occurs, `lenc_delta` and `renc_delta` will be accurate
     *
     * @param lenc_prev the previous left encoder value. This is modified inside the function
     *  to the value of the new reading
     * @param renc_prev the previous right encoder value. This is modified inside the function
     *  to the value of the new reading
     * @param lenc_delta the change in the left encoder. This is incremented inside the function
     * @param renc_delta the change in the right encoder. This is incremeneted inside the function
     */
    static void process_encoders(int &lenc_prev, int &renc_prev, int &lenc_delta, int &renc_delta);

    /**
     * @brief Get how far the left wheel has turned
     *
     * @return int - how far, in encoder units, the left create wheel has turned
     */
    int get_lenc_delta();

    /**
     * @brief Get how far the left wheel has turned
     *
     * @return int - how far, in encoder units, the left create wheel has turned
     */
    int get_renc_delta();

    /**
     * @brief Start the reading of encoder values.
     *
     */
    virtual void start();

protected:
    /**
     * @brief Function stub that BackgrounTask will call
     *
     */
    virtual void function();

private:
    EncoderSingleton(int updates_per_sec = get_config().getInt("roomba_updates_per_sec"));
    int lenc_prev;
    int renc_prev;
    int lenc_delta;
    int renc_delta;

    static void check_overflow(int16_t &temp, int &enc_prev, int &enc_delta);
    static std::shared_ptr<EncoderSingleton> _instance;
};

class AccelSingleton : public BackgroundTask
{
public:
    static AccelSingleton *instance();

    /**
     * @brief Get the current accelerometer reading, in accel units
     *
     * @return signed short
     */
    static signed short get_accel_val();
    /**
     * @brief Get the current accelerometer reading, in meters per second
     *
     * @return double
     */
    static double get_accel_ms();

    /**
     * @brief Get the current jolt (change in acceleration)
     * @details positive values are for forward jolt, negative values are for backwards jolt
     *
     * @return double
     */
    double get_jolt() const;

    /**
     * @brief Get the current velocity, in meters per second
     *
     * @return double
     */
    double get_velocity() const;
    /**
     * @brief Get the current position, in meters
     *
     * @return double
     */
    double get_position() const;

    /**
     * @brief Set the velocity to the given value
     *
     * @param velocity
     */
    void set_velocity(double velocity);

    /**
     * @brief Set the position to the given value
     *
     * @param position
     */
    void set_position(double position);

    /**
     * @brief Start the reading of accel values
     *
     */
    virtual void start();

protected:
    /**
     * @brief Function stub that BackgroundTask will call
     *
     */
    virtual void function();

private:
    AccelSingleton(int updates_per_second = get_config().getInt("accelerometer_updates_per_sec"));
    signed short accel_cur;
    signed short accel_prev;
    double jolt;
    double dt;
    double velocity_cur;
    double velocity_prev;
    double position;

    static std::shared_ptr<AccelSingleton> _instance;
};

/**
 * @brief Class that takes care of setting up the GyroSingleton for use
 * in client programs
 *
 */
class GyroSubscriber : public AngleSubscriber
{
public:
    /**
     * @brief Construct a new Gyro AngleSubscriber object
     * @details this will get the GyroSingleton accumulating, record the starting
     * angle that the brain was at at this moment in time, and whether or not the
     * GyroSingleton was already accumulating when this subscriber was created.
     *
     * @param updates_per_sec how many times to read from the gyroscope per sec
     */
    GyroSubscriber(int updates_per_sec);

    /**
     * @brief Destroy the Gyro AngleSubscriber object
     * @details if the GyroSingleton was already accumulating when this subscriber
     * was created, then the GyroSingleton will continue to accumulate. Else,
     * this will stop the accumulation.
     *
     */
    ~GyroSubscriber();

    /**
     * @brief Get the angle that the brain was at when this subscriber was created
     *
     * @return const double& - the angle that the brain was at when the GyroSubscriber was created
     */
    const double &get_start_angle();

    /**
     * @brief Get the angle relative to when this GyroSubscriber was created.
     * @details For instance, a value of 90.35 means that the brain has
     * turned 90.35 degrees CW since this GyroSubscriber was created.
     *
     * @return double - the angle the brain has turned through, in degrees.
     */
    virtual double get_relative_angle();

private:
    const double start_angle; ///< angle that the brain was at when this subscriber was created
    const double was_running; ///< whether or not the GyroSingleton was running when this subscriber was created
};

/**
 * @brief Class that takes care of setting up the EncoderSingleton for use
 * in client programs
 *
 */
class EncoderSubscriber : public AngleSubscriber, public DistanceSubscriber
{
public:
    /**
     * @brief Construct a new EncoderSubscriber object
     * @details this will get the EncoderSingleton running, record the starting
     * wheel encoder deltas for the create, and whether or not the
     * EncoderSingleton was already running when this subscriber was created.
     *
     * @param updates_per_sec how many times to read from the encoders per sec
     */

    EncoderSubscriber(int updates_per_sec);

    /**
     * @brief Destroy the EncoderSubscriber object
     * @details if the EncoderSingleton was already running when this subscriber
     * was created, then the EncoderSingleton will continue to run. Else,
     * this will stop the readings.
     *
     */
    ~EncoderSubscriber();

    /**
     * @brief Get the lenc_delta that the create was at when
     * this subscriber was created
     *
     * @return const int& - the initial delta, in encoder units, of the left wheel
     */
    const int &get_start_lenc_delta();

    /**
     * @brief Get the renc_delta that the create was at when
     * this subscriber was created
     *
     * @return const int& - the initial delta, in encoder units, of the right wheel
     */
    const int &get_start_renc_delta();

    /**
     * @brief Get how far the left create wheel has
     * traveled since this EncoderSubscriber was created,
     * in encoder units
     *
     * @return int - the delta, in encoder units, of the left wheel
     */
    int get_relative_lenc_delta();

    /**
     * @brief Get how far the right create wheel has
     * traveled since this EncoderSubscriber was created,
     * in encoder units
     *
     * @return int - the delta, in encoder units, of the right wheel
     */
    int get_relative_renc_delta();

    /**
     * @brief Get how far the left create wheel has
     * traveled since this EncoderSubscriber was created,
     * in mm
     *
     * @return double - how far, in mm, the left wheel has traveled
     */
    double get_relative_left_distance();

    /**
     * @brief Get how far the right create wheel has
     * traveled since this EncoderSubscriber was created,
     * in mm
     *
     * @return double - how far, in mm, the right wheel has traveled
     */
    double get_relative_right_distance();

    /**
     * @brief Get how far the create has traveled since
     * this EncoderSubscriber was created, in mm
     * @details this is equivalent to averaging
     * the left and right distances
     *
     * @return double - how far, in mm, the create has traveled
     */
    virtual double get_relative_distance();

    /**
     * @brief Get the angle relative to when this EncoderSubscriber
     * was created, in degrees.
     * @details For instance, a value of 90.35 means that the create has
     * turned 90.35 degrees CW since this EncoderSubscriber was created.
     * @details this is calculated using the following equation:
     *  `(left_mm + right_mm) / (dist_between_wheels_in_mm) * rad_2_deg`
     *
     * @return double - the angle the create has turned through, in degrees.
     */
    virtual double get_relative_angle();

private:
    const int start_lenc_delta;
    const int start_renc_delta;
    const bool was_running;
};

#endif
/**@}*/