#include "imu_movement.hpp"
#include "ihsboost/controllers/controllers.hpp"
#include "maneuvers.hpp"
#include "singletons.hpp"
#include <chrono>
#include <kipr/wombat.h>
#include <limits>
#include <thread>
#include <unordered_map>

// --------------------------------------------- GYRO FUNCTIONS
// --------------------------------------------- Functions in this section deal
// with gyroscope functions
// --------------------------------------------------------------------------------------------------------

void calibrate_ihsboost_gyro(int num_samples, uint16_t min_occurences) {
    // collect values
    std::unordered_map<signed short, uint16_t> map;
    for (int i = 0; i < num_samples; ++i) {
        signed short val = GYRO_FUNCTION();
        if (map.find(val) == map.end()) {
            map.insert(std::pair<signed short, uint16_t>(val, 1));
        }
        else {
            map[val] += 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // calculate our values
    signed short min = std::numeric_limits<signed short>::max();
    signed short max = std::numeric_limits<signed short>::min();
    uint16_t count = 0;
    double total = 0;
    for (std::pair<signed short, uint16_t> pair : map) {
        // filter values based on min occurences
        if (pair.second >= min_occurences) {
            // update vars that will be used for our average
            ++count;
            total += pair.first * pair.second;

            // update min and max
            min = std::min(min, pair.first);
            max = std::max(max, pair.first);
        }
    }
    total /= count; // average it

    // write changes to "bot-config.json"
    Config c;
    c.setInt("min_gyro_val", min);
    c.setInt("max_gyro_val", max);
    c.setDouble("mean_gyro_val", total);
    c.save();
}

void gyro_drive_straight(int speed,
                         std::function<bool()> stop_function,
                         bool stop,
                         double correction_proportion,
                         int updates_per_sec) {
    // setup gyro
    GyroSubscriber subscriber(updates_per_sec);

    drive_straight(&subscriber,
                   speed,
                   stop_function,
                   stop,
                   correction_proportion,
                   updates_per_sec);
}

void gyro_turn_degrees(Speed turn_speed, double degrees, int updates_per_sec) {
    GyroSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber, turn_speed, degrees, updates_per_sec);
}

void gyro_turn_degrees(int max_speed,
                       double degrees,
                       int min_speed,
                       double accel_per_sec,
                       int updates_per_sec) {
    // setup gyro
    GyroSubscriber subscriber(updates_per_sec);

    turn_degrees(&subscriber,
                 max_speed,
                 degrees,
                 min_speed,
                 accel_per_sec,
                 updates_per_sec);
}
