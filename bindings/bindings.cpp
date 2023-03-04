#include <ihsboost/all.hpp>
#include <boost/python.hpp>
#include <sstream>
#include <string>

// actually used functions
template <typename T, typename Container>
bool send_type(Communicator *communicator, Container l)
{
    T *msg = new T[boost::python::len(l)];
    for (ssize_t i = 0; i < boost::python::len(l); ++i)
    {
        msg[i] = boost::python::extract<T>(l[i]);
    }
    communicator->send_msg(communicator->create_msg<T>(msg, boost::python::len(l)));
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

std::string print_speed(Speed &s)
{
    std::ostringstream o;
    o << "(" << s.left << ", " << s.right << ")";
    return o.str();
}

BOOST_PYTHON_MODULE(ihs_bindings)
{
    using namespace boost::python;

    // communicate section
    // classes
    class_<Communicator, boost::noncopyable>("CommunicatorBase", no_init);
    class_<SocketServer, bases<Communicator>>("SocketServer", init<int, optional<int>>("Start a socket server on the given port. Messages will have max size of `max_msg_size`.\n\t@param port - what port to host on \n\t@param max_msg_size the maximum size for messages"));
    class_<SocketClient, bases<Communicator>>("SocketClient", init<const char *, int, optional<int>>("Open a socket client to the given ip and port. Messages will have max size of `max_msg_size`.\n\t@param ip - what ip to connect to\n\t@param port - what port to connect to \n\t@param max_msg_size the maximum size for messages"));
    class_<SHMCommunicator, bases<Communicator>>("SHMCommunicator", init<int, optional<int>>("Create a SHMCommunicator\n\t@param id - the id for the shared memory\n\t@param max_msg_size the maximum size for messages"));
    class_<PosixQCommunicator, bases<Communicator>>("PosixQCommunicator", init<const char *, optional<int, int>>("Create a PosixQCommunicator\n\t @param name - the name for this posix queue\n\t@param max_msgs the maximum number of messages that the posix q will be able to hold (defaults to 10)\n\t@param max_msg_size the maximum size for messages"));
    class_<SysVCommunicator, bases<Communicator>>("SysVCommunicator", init<int, optional<int>>("Construct a SysVCommunicator\n\t@param id - the id of the SysVCommunicator\n\t@param max_msg_size the maximum size for messages"));

    // ints
    def("receive_ints", receive_ints, "Receive a list of ints from the communicator.\n\t@param communicator - the communicator to receive messages through.\nExceptions:\n\tBoost.Python.ArgumentError - happens when passed arguments are of incorrect types\n\tTypeError - happens when c++ is unable to automatically typecast one of the arguments provided to the desired type");
    def("send_ints", send_ints, "Send a list of ints through the communicator.\n\t@param communicator - the communicator to send the message with\n\t@param ints - the list of ints to send\nExceptions:\n\tRuntimeError (BadBufCastException) - happens when trying to retreive the wrong type of values from the communicator");

    // doubles
    def("receive_doubles", receive_doubles, "Receive a list of doubles from the communicator.\n\t@param communicator - the communicator to receive messages through.\nExceptions:\n\tBoost.Python.ArgumentError - happens when passed arguments are of incorrect types\n\tTypeError - happens when c++ is unable to automatically typecast one of the arguments provided to the desired type");
    def("send_doubles", send_doubles, "Send a list of doubles through the communicator.\n\t@param communicator - the communicator to send the message with\n\t@param doubles - the list of doubles to send\nExceptions:\n\tRuntimeError (BadBufCastException) - happens when trying to retreive the wrong type of values from the communicator");

    // strings
    def("receive_string", receive_string, "Receive a string from the communicator.\n\t@param communicator - the communicator to receive messages through.\nExceptions:\n\tBoost.Python.ArgumentError - happens when passed arguments are of incorrect types\n\tTypeError - happens when c++ is unable to automatically typecast one of the arguments provided to the desired type");
    def("send_string", send_string, "Send a string through the communicator.\n\t@param communicator - the communicator to send the message with\n\t@param string - the string to send\nExceptions:\n\tRuntimeError (BadBufCastException) - happens when trying to retreive the wrong type of values from the communicator");

    // booleans
    def("receive_bools", receive_bools, "Receive a list of booleans from the communicator.\n\t@param communicator - the communicator to receive messages through.\nExceptions:\n\tBoost.Python.ArgumentError - happens when passed arguments are of incorrect types\n\tTypeError - happens when c++ is unable to automatically typecast one of the arguments provided to the desired type");
    def("send_bools", send_bools, "Send a list of booleans through the communicator.\n\t@param communicator - the communicator to send the message with\n\t@param bools - the list of booleans to send\nExceptions:\n\tRuntimeError (BadBufCastException) - happens when trying to retreive the wrong type of values from the communicator");

    // util section
    class_<Speed>("Speed", init<int, int>("Construct a Speed object\n\t@param left_speed - the speed of the left wheel\n\t@param right_speed - the speed of the right wheel"))
        .def("__eq__", &Speed::operator==)
        .def("__add__", &Speed::operator+)
        .def("__sub__", &Speed::operator-)
        .def_readwrite("left", &Speed::left)
        .def_readwrite("right", &Speed::right)
        .def("__repr__", print_speed);

    class_<Timer>("Timer", init<double>("Construct a Timer object\n\t@param time - how long to set the timer for, in seconds"))
        .def("__call__", &Timer::operator(), "Returns false until it is time to stop (when it has been `time` seconds)\n\t@return true - once it has been `time` seconds\n\t@return false - if it hasn't been `time` seconds yet")
        .def("done", &Timer::operator(), "Returns false until it is time to stop (when it has been `time` seconds)\n\t@return true - once it has been `time` seconds\n\t@return false - if it hasn't been `time` seconds yet");
    def("set_extra_config", set_extra_config, "Load tunable ihsboost variables from the given config file\n\t@details The extra config file doesn't need to contain all the tunable ihsboost variables\n\t@param config_file the name of the config file");
    def("set_default_config", set_default_config, "Load tunable ihsboost variables from the given config file\n\t@details The default config file should contain all the tunable ihsboost variables.\n\t@param config_file the name of the config file");

    // servos
    def("move_servo_slowly", move_servo_slowly, "Move a servo slowly to its goal position\n\t@param port the port where the servo is\n\t@param position the goal position to move the servo to\n\t@param speed how fast to move the servo, in ticks per second. Defaults to 200\n\t@param updates_per_sec How many updates to do per second, defaults to 100");

    // threading
    // N/A, python already has good threading

    // create_extra
    class_<CreateExtraController>("CreateExtraController", no_init)
        .def("run_main_brush", &CreateExtraController::run_main_brush, "Run the main brush at the specified speed\n\t@details positive values cause it to turn inwards,\n\tnegative values cause it to turn outwards\n\t\n\t@param speed How fast to turn the main brush, ranges from -127 to 127")
        .def("run_side_brush", &CreateExtraController::run_side_brush, "Run the side brush at the specified speed\n\t@details positive values cause it to turn CCW,\n\tnegative values cause it to turn CW\n\t\n\t@param speed How fast to turn the side brush, ranges from -127 to 127")
        .def("run_vacuum", &CreateExtraController::run_vacuum, "Run the vacuum at the specified speed\n\t@details only positive values are valid.\n\t\n\t@param speed How fast to turn the vacuum, ranges from 0 to 127");

    // movement
};