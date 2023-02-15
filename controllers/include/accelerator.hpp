/**
 * @file accelerator.hpp
 * @author Eliot Hall
 * @brief Acceleration controllers
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @defgroup accelerator_id Acceleration Classes
 * @{
 */
#ifndef ACCELERATOR_HPP
#define ACCELERATOR_HPP

/**
 * @brief Class for accelerating
 *
 */
class AccelerateController
{
public:
    /**
     * @brief Construct a new AccelerateController object
     *
     * @param from_speed the speed to accelerate from
     * @param to_speed the speed to accelerate to
     * @param accel_per_sec the acceleration per second
     * @param updates_per_sec how many updates to use per second
     */
    AccelerateController(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec) : _from_speed(from_speed), _to_speed(to_speed), _accel_per_sec(accel_per_sec), _updates_per_sec(updates_per_sec), _num_steps(0), _cur_speed(from_speed){};

    /**
     * @brief Updates the controller
     * @details Takes care of updating the speed; must be called in order to see any change in speed()
     */
    virtual void step() = 0;

    /**
     * @brief Returns the current speed
     * @return double - the current speed
     */
    double speed() { return _cur_speed; };

    /**
     * @brief Get how long to msleep for
     *
     * @return int - how long to msleep for
     */
    int get_msleep_time() { return 1000 / _updates_per_sec; }

    /**
     * @brief Return whether or not the controller is done
     *
     * @return true - the controller is done / exhausted.
     * @return false - the controller still has more to do
     */
    virtual bool done() = 0;

protected:
    int _from_speed;       ///< the speed to accelerate from
    int _to_speed;         ///< the speed to accelerate to
    double _accel_per_sec; ///< the acceleration per second
    int _updates_per_sec;  ///< how many updates to do per second

    int _num_steps;    ///< how many updates are necessary
    double _cur_speed; ///< the current speed value
};

/**
 * @brief Class for accelerating linearly
 *
 */
class LinearAccelerator : public AccelerateController
{
public:
    /**
     * @brief Construct a new LinearAccelerator object
     *
     * @param from_speed the speed to accelerate from
     * @param to_speed the speed to accelerate to
     * @param accel_per_sec the acceleration per second
     * @param updates_per_sec how many updates to use per second
     */
    LinearAccelerator(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec);

    /**
     * @brief Updates the controller
     * @details Takes care of updating the speed; must be called in order to see any change in speed()
     */
    virtual void step() override;

    /**
     * @brief Return whether or not the controller is done
     *
     * @return true - the controller is done / exhausted.
     * @return false - the controller still has more to do
     */
    virtual bool done() override;

private:
    int sign; ///< the sign/direction of the acceleration
};

/**
 * @brief Class for accelerating sinusoidally
 *
 */
class SinusoidalAccelerator : public AccelerateController
{
public:
    /**
     * @brief Construct a new Sinusoidal Accelerator object
     *
     * @param from_speed the speed to accelerate from
     * @param to_speed the speed to accelerate to
     * @param avg_accel_per_sec the acceleration per second
     * @param updates_per_sec how many updates to use per second
     */
    SinusoidalAccelerator(int from_speed, int to_speed, double avg_accel_per_sec, int updates_per_sec);

    /**
     * @brief Updates the controller
     * @details Takes care of updating the speed; must be called in order to see any change in speed()
     */
    virtual void step() override;

    /**
     * @brief Return whether or not the controller is done
     *
     * @return true - the controller is done / exhausted.
     * @return false - the controller still has more to do
     */
    virtual bool done() override;

private:
    int delta_speed;          ///< the change in speed
    double necessary_updates; ///< how many updates are necessary
    double sin_val;           ///< the current sin value
};

#endif
/**@}*/