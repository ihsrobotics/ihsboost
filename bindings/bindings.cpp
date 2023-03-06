#include <ihsboost/all.hpp>
#include <boost/python.hpp>
#include <sstream>
#include <string>
#include <functional>

namespace communicate_export
{
    template <typename T, typename Container>
    bool send_type(Communicator *communicator, Container l)
    {
        T *msg = new T[boost::python::len(l)];
        for (ssize_t i = 0; i < boost::python::len(l); ++i)
        {
            msg[i] = boost::python::extract<T>(l[i]);
        }
        communicator->send_msg(communicator->create_msg<T>(msg, static_cast<uint16_t>(boost::python::len(l))));
        delete[] msg;
        return true;
    }
    template <typename T>
    boost::python::list receive_type(Communicator *communicator)
    {
        boost::python::list ret;
        MessageBuf m = communicator->receive_msg();
        size_t len = m.get_length();
        for (size_t i = 0; i < len; ++i)
        {
            ret.append<T>(m.get_ptr_val<T>()[i]);
        }
        return ret;
    }

    // stubs
    bool send_ints(Communicator *communicator, boost::python::list ints)
    {
        return send_type<int, boost::python::list>(communicator, ints);
    }
    bool send_doubles(Communicator *communicator, boost::python::list doubles)
    {
        return send_type<double, boost::python::list>(communicator, doubles);
    }
    bool send_string(Communicator *communicator, boost::python::str string)
    {
        return send_type<char, boost::python::str>(communicator, string);
    }
    bool send_bools(Communicator *communicator, boost::python::list bools)
    {
        return send_type<bool, boost::python::list>(communicator, bools);
    }
    boost::python::list receive_ints(Communicator *communicator)
    {
        return receive_type<int>(communicator);
    }
    boost::python::list receive_doubles(Communicator *communicator)
    {
        return receive_type<double>(communicator);
    }
    boost::python::str receive_string(Communicator *communicator)
    {
        return boost::python::str("").join(receive_type<char>(communicator));
    }
    boost::python::list receive_bools(Communicator *communicator)
    {
        return receive_type<bool>(communicator);
    }
}; // namespace communicate_export

namespace util_export
{
    std::string print_speed(Speed &s)
    {
        std::ostringstream o;
        o << "(" << s.left << ", " << s.right << ")";
        return o.str();
    }
}; // namespace util_export

namespace movement_export
{
    void (*encoder_drive_straight_cm)(int, double, bool, int, double, double, int) = encoder_drive_straight;
    void (*encoder_drive_straight_fn)(int, std::function<bool()>, bool, double, int) = encoder_drive_straight;
    void (*encoder_turn_degrees_v1)(Speed, double, int) = encoder_turn_degrees;
    void (*encoder_turn_degrees_v2)(int, double, int, double, int) = encoder_turn_degrees;
}

BOOST_PYTHON_MODULE(ihs_bindings)
{
    using namespace boost::python;
    using namespace communicate_export;
    using namespace util_export;
    using namespace movement_export;

    // communicate section
    // classes
    class_<Communicator, boost::noncopyable>("CommunicatorBase", no_init);
    class_<SocketServer, bases<Communicator>>("SocketServer", init<int, optional<int>>((arg("port"), arg("max_msg_size"))));
    class_<SocketClient, bases<Communicator>>("SocketClient", init<const char *, int, optional<int>>((arg("ip"), arg("max_msg_size"))));
    class_<SHMCommunicator, bases<Communicator>>("SHMCommunicator", init<int, optional<int>>((arg("id"), arg("max_msg_size"))));
    class_<PosixQCommunicator, bases<Communicator>>("PosixQCommunicator", init<const char *, optional<int, int>>((arg("name"), arg("max_msgs"), arg("max_msg_size"))));
    class_<SysVCommunicator, bases<Communicator>>("SysVCommunicator", init<int, optional<int>>((arg("id"), arg("max_msg_size"))));

    // ints
    def("receive_ints", receive_ints);
    def("send_ints", send_ints);

    // doubles
    def("receive_doubles", receive_doubles);
    def("send_doubles", send_doubles);

    // strings
    def("receive_string", receive_string);
    def("send_string", send_string);

    // booleans
    def("receive_bools", receive_bools);
    def("send_bools", send_bools);

    // util section
    class_<Speed>("Speed", init<int, int>((arg("left_speed"), arg("right_speed"))))
        .def("__eq__", &Speed::operator==)
        .def("__ne__", &Speed::operator!=)
        .def("__add__", &Speed::operator+)
        .def("__sub__", &Speed::operator-)
        .def_readwrite("left", &Speed::left)
        .def_readwrite("right", &Speed::right)
        .def("__repr__", print_speed);
    class_<Timer>("Timer", init<double>(arg("time")))
        .def("__call__", &Timer::operator())
        .def("done", &Timer::operator());
    enum_<Cliff>("Cliff")
        .value("LCliff", Cliff::LCliff)
        .value("LFCliff", Cliff::LFCliff)
        .value("RFCliff", Cliff::RFCliff)
        .value("RCliff", Cliff::RCliff);
    enum_<LineSide>("LineSide")
        .value("LeftSide", LineSide::LeftSide)
        .value("RightSide", LineSide::RightSide);
    def("set_extra_config", set_extra_config);
    def("set_default_config", set_default_config);

    // servos
    def("move_servo_slowly", move_servo_slowly, (arg("port"), arg("position"), arg("speed") = get_config().getInt("servo_movement_speed"), arg("updates_per_sec") = get_config().getInt("servo_updates_per_sec")));

    // threading
    // N/A, python already has good threading

    // create_extra
    class_<CreateExtraController>("CreateExtraController", init<>())
        .def("run_main_brush", &CreateExtraController::run_main_brush)
        .def("run_side_brush", &CreateExtraController::run_side_brush)
        .def("run_vacuum", &CreateExtraController::run_vacuum);

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
};