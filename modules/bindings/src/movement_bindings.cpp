#include "modules.hpp"

#ifdef build_movement
#include "movement.hpp"
#include <boost/python.hpp>
#include <functional>

namespace movement_export {
using namespace boost::python;

void gyro_drive_straight_fn(int sp, object fn, bool stop, double c, int u) {
    gyro_drive_straight(sp, fn, stop, c, u);
}

void line_follow_basic_fn(
    int sp, Cliff cl, LineSide l, object fn, bool st, double co, int b, int u) {
    line_follow_basic(sp, cl, l, fn, st, co, b, u);
}
void line_follow_accelerate_linear_fn(int f,
                                      int t,
                                      Cliff cl,
                                      LineSide l,
                                      object fn,
                                      bool s,
                                      double co,
                                      double a,
                                      int b,
                                      int u) {
    line_follow_accelerate_linear(f, t, cl, l, fn, s, co, a, b, u);
}
void line_follow_accelerate_sinusoidal_fn(int f,
                                          int t,
                                          Cliff cl,
                                          LineSide l,
                                          object fn,
                                          bool s,
                                          double co,
                                          double a,
                                          int b,
                                          int u) {
    line_follow_accelerate_sinusoidal(f, t, cl, l, fn, s, co, a, b, u);
}

double (*encoder_drive_straight_cm)(
    int, double, bool, int, double, double, int) = encoder_drive_straight;
double encoder_drive_straight_fn(int s, object fn, bool stop, double c, int u) {
    return encoder_drive_straight(s, fn, stop, c, u);
}

void (*encoder_turn_degrees_v1)(Speed, double, int) = encoder_turn_degrees;
void (*encoder_turn_degrees_v2)(int, double, int, double, int) =
    encoder_turn_degrees;
void (*gyro_turn_degrees_v1)(Speed, double, int) = gyro_turn_degrees;
void (*gyro_turn_degrees_v2)(int, double, int, double, int) = gyro_turn_degrees;

double get_start_angle(GyroSubscriber *g) { return g->get_start_angle(); }
int get_start_lenc_delta(EncoderSubscriber *e) {
    return e->get_start_lenc_delta();
}
int get_start_renc_delta(EncoderSubscriber *e) {
    return e->get_start_renc_delta();
}
} // namespace movement_export

void export_movement() {
    using namespace movement_export;
    using namespace boost::python;

    // movement
    class_<GyroSubscriber>("GyroSubscriber", init<int>())
        .def("get_start_angle", get_start_angle)
        .def("get_relative_angle", &GyroSubscriber::get_relative_angle);
    class_<EncoderSubscriber>("EncoderSubscriber", init<int>())
        .def("get_start_lenc_delta", get_start_lenc_delta)
        .def("get_start_renc_delta", get_start_renc_delta)
        .def("get_relative_lenc_delta",
             &EncoderSubscriber::get_relative_lenc_delta)
        .def("get_relative_renc_delta",
             &EncoderSubscriber::get_relative_renc_delta)
        .def("get_relative_left_distance",
             &EncoderSubscriber::get_relative_left_distance)
        .def("get_relative_right_distance",
             &EncoderSubscriber::get_relative_right_distance)
        .def("get_relative_distance", &EncoderSubscriber::get_relative_distance)
        .def("get_relative_angle", &EncoderSubscriber::get_relative_angle);
    def("accelerate_forward_linear",
        accelerate_forward_linear,
        (arg("from_speed"),
         arg("to_speed"),
         arg("accel_per_sec") = get_config().getDouble("linear_accel"),
         arg("updates_per_sec") =
             get_config().getInt("accelerate_updates_per_sec")));
    def("accelerate_forward_sin",
        accelerate_forward_sin,
        (arg("from_speed"),
         arg("to_speed"),
         arg("avg_accel_per_sec") = get_config().getDouble("sinusoidal_accel"),
         arg("updates_per_sec") =
             get_config().getInt("accelerate_updates_per_sec")));
    def("accelerate_linear",
        accelerate_linear,
        (arg("from_speed"),
         arg("to_speed"),
         arg("accel_per_sec") = get_config().getDouble("linear_accel"),
         arg("updates_per_sec") =
             get_config().getInt("accelerate_updates_per_sec")));
    def("accelerate_sinusoidal",
        accelerate_sinusoidal,
        (arg("from_speed"),
         arg("to_speed"),
         arg("accel_per_sec") = get_config().getDouble("sinusoidal_accel"),
         arg("updates_per_sec") =
             get_config().getInt("accelerate_updates_per_sec")));
    def("gyro_drive_straight",
        gyro_drive_straight_fn,
        (arg("speed"),
         arg("stop_function"),
         arg("stop") = get_config().getBool("gyro_stop"),
         arg("correction_proportion") =
             get_config().getDouble("gyro_correction_proportion"),
         arg("updates_per_sec") = get_config().getInt("gyro_updates_per_sec")));
    def("gyro_turn_degrees",
        gyro_turn_degrees_v1,
        (arg("turn_speed"),
         arg("degrees"),
         arg("updates_per_sec") = get_config().getInt("gyro_updates_per_sec")));
    def("gyro_turn_degrees_v2",
        gyro_turn_degrees_v2,
        (arg("max_speed"),
         arg("degrees"),
         arg("min_speed") = get_config().getInt("gyro_min_speed"),
         arg("accel_per_sec") = get_config().getDouble("gyro_accel_per_sec"),
         arg("updates_per_sec") = get_config().getInt("gyro_updates_per_sec")));
    def("is_black", is_black, (arg("val")));
    def("is_white", is_white, (arg("val")));
    def("line_follow_basic",
        line_follow_basic_fn,
        (arg("speed"),
         arg("cliff_sensor"),
         arg("line_side"),
         arg("stop_condition"),
         arg("stop") = get_config().getBool("line_follow_stop"),
         arg("correction_proportion") =
             get_config().getDouble("line_follow_correction_proportion"),
         arg("black_val") = get_config().getInt("black"),
         arg("updates_per_sec") =
             get_config().getInt("line_follow_updates_per_sec")));
    def("line_follow_accelerate_linear",
        line_follow_accelerate_linear_fn,
        (arg("from_speed"),
         arg("to_speed"),
         arg("cliff_sensor"),
         arg("line_side"),
         arg("stop_condition"),
         arg("stop") = get_config().getBool("line_follow_stop"),
         arg("correction_proportion") =
             get_config().getDouble("line_follow_correction_proportion"),
         arg("accel_per_sec") =
             get_config().getDouble("line_follow_linear_acceleration"),
         arg("black_val") = get_config().getInt("black"),
         arg("updates_per_sec") =
             get_config().getInt("line_follow_updates_per_sec")));
    def("line_follow_accelerate_sinusoidal",
        line_follow_accelerate_sinusoidal_fn,
        (arg("from_speed"),
         arg("to_speed"),
         arg("cliff_sensor"),
         arg("line_side"),
         arg("stop_condition"),
         arg("stop") = get_config().getBool("line_follow_stop"),
         arg("correction_proportion") =
             get_config().getDouble("line_follow_correction_proportion"),
         arg("accel_per_sec") =
             get_config().getDouble("line_follow_sinusoidal_acceleration"),
         arg("black_val") = get_config().getInt("black"),
         arg("updates_per_sec") =
             get_config().getInt("line_follow_updates_per_sec")));
    def("align_with_black",
        align_with_black,
        (arg("speed"),
         arg("correction_speed"),
         arg("cliff_sensor_l"),
         arg("cliff_sensor_r"),
         arg("stop") = get_config().getBool("align_stop"),
         arg("updates_per_second") =
             get_config().getInt("align_updates_per_sec")));
    def("align_with_white",
        align_with_white,
        (arg("speed"),
         arg("correction_speed"),
         arg("cliff_sensor_l"),
         arg("cliff_sensor_r"),
         arg("stop") = get_config().getBool("align_stop"),
         arg("updates_per_second") =
             get_config().getInt("align_updates_per_sec")));
    def("rotate",
        rotate,
        (arg("leftWheelSpeed"),
         arg("rightWheelSpeed"),
         arg("angle"),
         arg("left_wheel_units") =
             get_config().getDouble("roomba_left_wheel_units"),
         arg("right_wheel_units") =
             get_config().getDouble("roomba_right_wheel_units")));
    def("encoder_drive_straight_cm",
        encoder_drive_straight_cm,
        (arg("speed"),
         arg("cm"),
         arg("stop") = get_config().getBool("roomba_stop"),
         arg("min_speed") = get_config().getInt("roomba_min_speed"),
         arg("correction_proportion") =
             get_config().getDouble("roomba_correction_proportion"),
         arg("accel_per_sec") = get_config().getDouble("roomba_accel_per_sec"),
         arg("updates_per_sec") =
             get_config().getInt("roomba_updates_per_sec")));
    def("encoder_drive_straight_fn",
        encoder_drive_straight_fn,
        (arg("speed"),
         arg("condition"),
         arg("stop") = get_config().getBool("roomba_stop"),
         arg("correction_proportion") =
             get_config().getDouble("roomba_correction_proportion"),
         arg("updates_per_sec") =
             get_config().getInt("roomba_updates_per_sec")));
    def("encoder_drive_straight_pid",
        encoder_drive_straight_pid,
        (arg("speed"),
         arg("cm"),
         arg("proportional_coefficient"),
         arg("integral_coefficient"),
         arg("derivative_coefficient"),
         arg("stop") = get_config().getBool("roomba_stop"),
         arg("min_speed") = get_config().getInt("roomba_min_speed"),
         arg("accel_per_sec") = get_config().getDouble("roomba_accel_per_sec"),
         arg("updates_per_sec") =
             get_config().getInt("roomba_updates_per_sec")));
    def("encoder_turn_degrees",
        encoder_turn_degrees_v1,
        (arg("turn_speed"),
         arg("degrees"),
         arg("updates_per_sec") =
             get_config().getInt("roomba_updates_per_sec")));
    def("encoder_turn_degrees_v2",
        encoder_turn_degrees_v2,
        (arg("max_speed"),
         arg("degrees"),
         arg("min_speed") = get_config().getInt("roomba_min_speed"),
         arg("accel_per_sec") = get_config().getDouble("roomba_accel_per_sec"),
         arg("updates_per_sec") =
             get_config().getInt("roomba_updates_per_sec")));
}
#endif