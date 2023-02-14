#include <ihsboost/all.hpp>
#include <boost/python.hpp>

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
    delete msg;
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

BOOST_PYTHON_MODULE(ihs_communicate)
{
    using namespace boost::python;

    class_<Communicator, boost::noncopyable>("CommunicatorBase", no_init);
    class_<SocketServer, bases<Communicator>>("SocketServer", init<int, optional<int>>("Start a socket server on the given port. Messages will have max size of `max_msg_size`.\n\t@param port - what port to host on \n\t@param max_msg_size the maximum size for messages"));
    class_<SocketClient, bases<Communicator>>("SocketClient", init<const char *, int, optional<int>>("Open a socket client to the given ip and port. Messages will have max size of `max_msg_size`.\n\t@param ip - what ip to connect to\n\t@param port - what port to connect to \n\t@param max_msg_size the maximum size for messages"));
    class_<SHMCommunicator, bases<Communicator>>("SHMCommunicator", init<int, optional<int>>("Create a SHMCommunicator\n\t@param id - the id for the shared memory\n\t@param max_msg_size the maximum size for messages"));
    class_<PosixQCommunicator, bases<Communicator>>("PosixQCommunicator", init<const char *, optional<int, int>>("Create a PosixQCommunicator\n\t @param name - the name for this posix queue\n\t@param max_msgs the maximum number of messages that the posix q will be able to hold (defaults to 10)\n\t@param max_msg_size the maximum size for messages"));
    class_<SysVCommunicator, bases<Communicator>>("SysVCommunicator", init<int, optional<int>>("Construct a SysVCommunicator\n\t@param id - the id of the SysVCommunicator\n\t@param max_msg_size the maximum size for messages"));

    def("receive_ints", receive_ints);
    def("send_ints", send_ints);

    def("receive_doubles", receive_doubles);
    def("send_doubles", send_doubles);

    def("receive_string", receive_string);
    def("send_string", send_string);
};