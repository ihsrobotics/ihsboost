#include <ihsboost/movement.hpp>
#include <boost/python.hpp>
#include <functional>

namespace movement_export
{
    double (*encoder_drive_straight_cm)(int, double, bool, int, double, double, int) = encoder_drive_straight;
    double (*encoder_drive_straight_fn)(int, std::function<bool()>, bool, double, int) = encoder_drive_straight;
    void (*encoder_turn_degrees_v1)(Speed, double, int) = encoder_turn_degrees;
    void (*encoder_turn_degrees_v2)(int, double, int, double, int) = encoder_turn_degrees;
} // namespace movement_export

void export_movement()
{
    using namespace movement_export;
    using namespace boost::python;

    // movement
    def("accelerate_forward_linear", accelerate_forward_linear, (arg("from_speed"), arg("to_speed"), arg("accel_per_sec"), arg("updates_per_sec")));
    def("accelerate_forward_sin", accelerate_forward_sin, (arg("from_speed"), arg("to_speed"), arg("avg_accel_per_sec"), arg("updates_per_sec")));
    def("accelerate_linear", accelerate_linear, (arg("from_speed"), arg("to_speed"), arg("accel_per_sec"), arg("updates_per_sec")));
    def("accelerate_sinusoidal", accelerate_sinusoidal, (arg("from_speed"), arg("to_speed"), arg("accel_per_sec"), arg("updates_per_sec")));
    def("gyro_drive_straight", gyro_drive_straight, (arg("from_speed"), arg("to_speed"), arg("stop_function"), arg("correction_proportion") = get_config().getDouble("gyro_correction_proportion"), arg("accel_per_sec") = get_config().getDouble("gyro_linear_accel"), arg("updates_per_sec") = get_config().getInt("gyro_updates_per_sec")), "drive straight for `seconds` seconds using the gyroscope");
    def("gyro_turn_degrees", gyro_turn_degrees, (arg("from_speed"), arg("to_speed"), arg("degrees"), arg("accel_per_sec") = get_config().getDouble("gyro_linear_accel"), arg("updates_per_sec") = get_config().getInt("gyro_updates_per_sec")), "turn a certain number of degrees");
    def("gyro_turn_degrees_v2", gyro_turn_degrees_v2, (arg("max_speed"), arg("degrees"), arg("min_speed") = get_config().getInt("gyro_turn_v2_min_speed"), arg("accel_per_sec") = get_config().getDouble("gyro_linear_accel"), arg("updates_per_sec") = get_config().getInt("gyro_updates_per_sec")));
    def("is_black", is_black, (arg("val")));
    def("is_white", is_white, (arg("val")));
    def("line_follow_basic", line_follow_basic, (arg("speed"), arg("cliff_sensor"), arg("line_side"), arg("stop_condition"), arg("stop") = get_config().getBool("line_follow_stop"), arg("correction_proportion") = get_config().getDouble("line_follow_correction_proportion"), arg("black_val") = get_config().getInt("black"), arg("updates_per_sec") = get_config().getInt("line_follow_updates_per_sec")));
    def("line_follow_accelerate_linear", line_follow_accelerate_linear, (arg("from_speed"), arg("to_speed"), arg("cliff_sensor"), arg("line_side"), arg("stop_condition"), arg("stop") = get_config().getBool("line_follow_stop"), arg("correction_proportion") = get_config().getDouble("line_follow_correction_proportion"), arg("accel_per_sec") = get_config().getDouble("line_follow_linear_acceleration"), arg("black_val") = get_config().getInt("black"), arg("updates_per_sec") = get_config().getInt("line_follow_updates_per_sec")));
    def("line_follow_accelerate_sinusoidal", line_follow_accelerate_sinusoidal, (arg("from_speed"), arg("to_speed"), arg("cliff_sensor"), arg("line_side"), arg("stop_condition"), arg("stop") = get_config().getBool("line_follow_stop"), arg("correction_proportion") = get_config().getDouble("line_follow_correction_proportion"), arg("accel_per_sec") = get_config().getDouble("line_follow_sinusoidal_acceleration"), arg("black_val") = get_config().getInt("black"), arg("updates_per_sec") = get_config().getInt("line_follow_updates_per_sec")));
    def("align_with_black", align_with_black, (arg("speed"), arg("correction_speed"), arg("cliff_sensor_l"), arg("cliff_sensor_r"), arg("stop") = get_config().getBool("align_stop"), arg("updates_per_second") = get_config().getInt("align_updates_per_sec")));
    def("align_with_white", align_with_white, (arg("speed"), arg("correction_speed"), arg("cliff_sensor_l"), arg("cliff_sensor_r"), arg("stop") = get_config().getBool("align_stop"), arg("updates_per_second") = get_config().getInt("align_updates_per_sec")));
    def("rotate", rotate, (arg("leftWheelSpeed"), arg("rightWheelSpeed"), arg("angle"), arg("left_wheel_units") = get_config().getDouble("roomba_left_wheel_units"), arg("right_wheel_units") = get_config().getDouble("roomba_right_wheel_units")));
    def("encoder_drive_straight_cm", encoder_drive_straight_cm, (arg("speed"), arg("cm"), arg("stop") = get_config().getBool("roomba_stop"), arg("min_speed") = get_config().getInt("roomba_min_speed"), arg("correction_proportion") = get_config().getDouble("roomba_correction_proportion"), arg("accel_per_sec") = get_config().getDouble("roomba_accel_per_sec"), arg("updates_per_sec") = get_config().getInt("roomba_updates_per_sec")));
    def("encoder_drive_straight_fn", encoder_drive_straight_fn, (arg("speed"), arg("condition"), arg("stop") = get_config().getBool("roomba_stop"), arg("correction_proportion") = get_config().getDouble("roomba_correction_proportion"), arg("updates_per_sec") = get_config().getInt("roomba_updates_per_sec")));
    def("encoder_drive_straight_pid", encoder_drive_straight_pid, (arg("speed"), arg("cm"), arg("proportional_coefficient"), arg("integral_coefficient"), arg("derivative_coefficient"), arg("stop") = get_config().getBool("roomba_stop"), arg("min_speed") = get_config().getInt("roomba_min_speed"), arg("accel_per_sec") = get_config().getDouble("roomba_accel_per_sec"), arg("updates_per_sec") = get_config().getInt("roomba_updates_per_sec")));
    def("encoder_turn_degrees", encoder_turn_degrees_v1, (arg("turn_speed"), arg("degrees"), arg("updates_per_sec") = get_config().getInt("roomba_updates_per_sec")));
    def("encoder_turn_degrees_v2", encoder_turn_degrees_v2, (arg("max_speed"), arg("degrees"), arg("min_speed") = get_config().getInt("roomba_min_speed"), arg("accel_per_sec") = get_config().getDouble("roomba_accel_per_sec"), arg("updates_per_sec") = get_config().getInt("roomba_updates_per_sec")));
}