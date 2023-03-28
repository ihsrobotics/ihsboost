/**
 * @file controllers.hpp
 * @author Eliot Hall
 * @brief Movement controllers
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @defgroup controllers_id Movement Control Classes
 * @{
 */
#ifndef IHSBOOST_CONTROLLERS_HPP
#define IHSBOOST_CONTROLLERS_HPP

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
    const double speed() const;

    /**
     * @brief Get how long to msleep for
     *
     * @return int - how long to msleep for
     */
    const int get_msleep_time() const;

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
class LinearController : public AccelerateController
{
public:
    /**
     * @brief Construct a new LinearController object
     *
     * @param from_speed the speed to accelerate from
     * @param to_speed the speed to accelerate to
     * @param accel_per_sec the acceleration per second
     * @param updates_per_sec how many updates to use per second
     */
    LinearController(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec);

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
class SinusoidalController : public AccelerateController
{
public:
    /**
     * @brief Construct a new Sinusoidal Controller object
     *
     * @param from_speed the speed to accelerate from
     * @param to_speed the speed to accelerate to
     * @param avg_accel_per_sec the acceleration per second
     * @param updates_per_sec how many updates to use per second
     */
    SinusoidalController(int from_speed, int to_speed, double avg_accel_per_sec, int updates_per_sec);

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
};

/**
 * @brief A basic PID Controller
 *
 */
class PIDController
{
public:
    /**
     * @brief Construct a new PIDController object
     * @details uses the formula \f[u(t) = K_p e(t) + K_i \int e(t)dt + K_d \frac{\mathrm{d} e}{\mathrm{d} x}\f]
     *
     * @param Kp the coefficient for proportionality to e(t)
     * @param Ki the coefficient for the integral of e(t)
     * @param Kd the coefficient for the derivative of e(t)
     * @param updates_per_second how many updates per second are done (used to calculate dt)
     */
    PIDController(double Kp, double Ki, double Kd, int updates_per_second);

    /**
     * @brief Update u(t) with the given error
     *
     * @param error the error at time t (the value of e(t))
     */
    void step(double error);

    /**
     * @brief Calculate the error (value - goal) and step using that error
     *
     * @param value the read value
     * @param goal the goal value
     */
    void step(double value, double goal);

    /**
     * @brief Return the speed calculated by the PIDController
     *
     * @return double - the speed
     */
    const double speed() const;

private:
    double Kp; ///< coefficient for proportionality
    double Ki; ///< coefficient for the integral
    double Kd; ///< coefficient for the derivative

    double error_integral; ///< holds the value of the integral
    double past_error;     ///< holds the past error value
    double dt;             ///< dt (delta time)

    double cur_speed; ///< cur speed, determined by u(t)
};

#endif
/**@}*/