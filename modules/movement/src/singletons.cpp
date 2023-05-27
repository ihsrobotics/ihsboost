#include "singletons.hpp"
#include "movement_constants.hpp"
#include <kipr/wombat.h>
#include <limits>

#define between(val, a, b) ((a >= val && val >= b) || (b >= val && val >= a))

// --------------------------------------------- GYRO SINGLETON
// --------------------------------------------- Functions in this section deal
// with the Gyroscope Singleton
// --------------------------------------------------------------------------------------------------------
GyroSingleton::GyroSingleton(int updates_per_sec)
    : Accumulator(get_gyro_angle, updates_per_sec){};
double GyroSingleton::get_gyro_val() {
    signed short val = GYRO_FUNCTION();
    return between(val,
                   get_config().getDouble("min_gyro_val"),
                   get_config().getDouble("max_gyro_val"))
               ? 0
               : (static_cast<double>(val) -
                  get_config().getDouble("mean_gyro_val")) *
                     get_config().getDouble("gyro_cw_multiplier");
}
double GyroSingleton::get_gyro_angle() {
    return get_gyro_val() / get_config().getDouble("raw_to_360_degrees");
}
GyroSingleton *GyroSingleton::instance() {
    if (_instance.get() == nullptr) {
        _instance = std::shared_ptr<GyroSingleton>(new GyroSingleton());
    }
    return _instance.get();
}

std::shared_ptr<GyroSingleton> GyroSingleton::_instance = nullptr;

// --------------------------------------------- GYRO SUBSCRIBER
// --------------------------------------------- Functions in this section deal
// with the gyroscope subscriber class
// --------------------------------------------------------------------------------------------------------
GyroSubscriber::GyroSubscriber(int updates_per_sec)
    : start_angle(GyroSingleton::instance()->get_accumulator()),
      was_running(GyroSingleton::instance()->is_running()) {
    // setup gyro
    GyroSingleton::instance()->set_updates_per_sec(updates_per_sec);

    // start accumulating
    GyroSingleton::instance()->start();
}
GyroSubscriber::~GyroSubscriber() {
    if (!was_running) {
        GyroSingleton::instance()->stop();
    }
}
const double &GyroSubscriber::get_start_angle() { return start_angle; }
double GyroSubscriber::get_relative_angle() {
    return GyroSingleton::instance()->get_accumulator() - start_angle;
}

// --------------------------------------------- ENCODER SINGLETON
// --------------------------------------------- Functions in this section deal
// with the Encoder Singleton
// --------------------------------------------------------------------------------------------------------
EncoderSingleton::EncoderSingleton(int updates_per_sec)
    : BackgroundTask(updates_per_sec),
      lenc_prev(0),
      renc_prev(0),
      lenc_delta(0),
      renc_delta(0) {}
EncoderSingleton *EncoderSingleton::instance() {
    if (_instance.get() == nullptr) {
        _instance = std::shared_ptr<EncoderSingleton>(new EncoderSingleton());
    }
    return _instance.get();
}
void EncoderSingleton::read_encoders(int &lenc, int &renc) {
    // read values
    int16_t l_temp = 0, r_temp = 0;
    _create_get_raw_encoders(&l_temp, &r_temp);

    // set values
    lenc = l_temp;
    renc = r_temp;
}
void EncoderSingleton::check_overflow(int16_t &temp,
                                      int &enc_prev,
                                      int &enc_delta) {
    // this function aims to stop the effects of overflow
    // it detects overflow with the following:
    // check if this jumped a huge amount (sign of overflow)
    // the .8 is an arbitrary number just to make it more accepting

    // check if it overflowed
    if (abs(static_cast<int>(temp) - enc_prev) >
        std::numeric_limits<u_short>::max() * .8)
    {
        // attempt to correct it
        // case of underflow
        if (enc_prev < 0 && temp > 0) {
            // subtract the positive difference that it traveled on the positive
            // side of the underflow
            enc_delta -= std::numeric_limits<int16_t>::max() - temp;
            // subtract the difference that it traveled on the negative side of
            // the underflow
            enc_delta -= enc_prev - std::numeric_limits<int16_t>::min();
        }
        // case of overflow
        if (enc_prev > 0 && temp < 0) {
            // add the positive difference that it traveled on the positive side
            // of the overflow
            enc_delta += std::numeric_limits<int16_t>::max() - enc_prev;
            // add the positive difference that it traveled on the negative side
            // of the underflow
            enc_delta += temp - std::numeric_limits<int16_t>::min();
        }
    }
    else {
        // if didn't overflow, just continue as normal
        enc_delta += temp - enc_prev;
    }
}
void EncoderSingleton::process_encoders(int &lenc_prev,
                                        int &renc_prev,
                                        int &lenc_delta,
                                        int &renc_delta) {
    // read values
    int16_t l_temp = 0, r_temp = 0;
    _create_get_raw_encoders(&l_temp, &r_temp);

    check_overflow(l_temp, lenc_prev, lenc_delta);
    check_overflow(r_temp, renc_prev, renc_delta);

    // set the "previous" variables to the new previous values
    lenc_prev = l_temp;
    renc_prev = r_temp;
}
int EncoderSingleton::get_lenc_delta() { return lenc_delta; }
int EncoderSingleton::get_renc_delta() { return renc_delta; }
void EncoderSingleton::function() {
    process_encoders(lenc_prev, renc_prev, lenc_delta, renc_delta);
}
void EncoderSingleton::start() {
    if (!is_running()) {
        read_encoders(lenc_prev, renc_prev);
    }
    BackgroundTask::start();
}

std::shared_ptr<EncoderSingleton> EncoderSingleton::_instance = nullptr;

// --------------------------------------------- ENCODER SUBSCRIBER
// --------------------------------------------- Functions in this section deal
// with the encoder subscriber
// --------------------------------------------------------------------------------------------------------
EncoderSubscriber::EncoderSubscriber(int updates_per_sec)
    : start_lenc_delta(EncoderSingleton::instance()->get_lenc_delta()),
      start_renc_delta(EncoderSingleton::instance()->get_renc_delta()),
      was_running(EncoderSingleton::instance()->is_running()) {
    // setup
    EncoderSingleton::instance()->set_updates_per_sec(updates_per_sec);

    // start
    EncoderSingleton::instance()->start();
};
EncoderSubscriber::~EncoderSubscriber() {
    if (!was_running) {
        EncoderSingleton::instance()->stop();
    }
}
const int &EncoderSubscriber::get_start_lenc_delta() {
    return start_lenc_delta;
}
const int &EncoderSubscriber::get_start_renc_delta() {
    return start_renc_delta;
}
int EncoderSubscriber::get_relative_lenc_delta() {
    return EncoderSingleton::instance()->get_lenc_delta() - start_lenc_delta;
}
int EncoderSubscriber::get_relative_renc_delta() {
    return EncoderSingleton::instance()->get_renc_delta() - start_renc_delta;
}
double EncoderSubscriber::get_relative_left_distance() {
    return get_relative_lenc_delta() * ENC_2_MM;
}
double EncoderSubscriber::get_relative_right_distance() {
    return get_relative_renc_delta() * ENC_2_MM;
}
double EncoderSubscriber::get_relative_distance() {
    return (get_relative_left_distance() + get_relative_right_distance()) / 2.0;
}
double EncoderSubscriber::get_relative_angle() {
    return (get_relative_left_distance() - get_relative_right_distance()) /
           (DIST_BETWEEN_WHEEL * 10) * rad2deg_mult;
}
