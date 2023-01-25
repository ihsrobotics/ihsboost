#pragma once

class AccelerateController
{
public:
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
    int _from_speed;
    int _to_speed;
    double _accel_per_sec;
    int _updates_per_sec;

    int _num_steps;
    double _cur_speed;
};

class LinearAccelerator : public AccelerateController
{
public:
    LinearAccelerator(int from_speed, int to_speed, double accel_per_sec, int updates_per_sec);
    virtual void step() override;
    virtual bool done() override;

private:
    int sign;
};

class SinusoidalAccelerator : public AccelerateController
{
public:
    SinusoidalAccelerator(int from_speed, int to_speed, double avg_accel_per_sec, int updates_per_sec);
    virtual void step() override;
    virtual bool done() override;

private:
    int delta_speed;
    double necessary_updates;
    double sin_val;
};
