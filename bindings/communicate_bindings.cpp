#include "communicate.hpp"
#include <boost/python.hpp>

namespace communicate_export
{
    // send values as a list
    template <typename T, typename Container>
    bool send_values(Communicator *communicator, Container l)
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
    // send single values
    template <typename T>
    bool send_val(Communicator *communicator, T val)
    {
        communicator->send_msg(communicator->create_msg<T>(val));
        return true;
    }

    // receive multiple values
    template <typename T>
    boost::python::list receive_values(Communicator *communicator)
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
    // receive single value
    template <typename T>
    T receive_val(Communicator *communicator)
    {
        MessageBuf m = communicator->receive_msg();
        return m.get_val<T>();
    }

    // stubs
    bool send_ints(Communicator *communicator, boost::python::list ints)
    {
        return send_values<int, boost::python::list>(communicator, ints);
    }
    bool send_int(Communicator *communicator, int _int)
    {
        return send_val<int>(communicator, _int);
    }
    bool send_doubles(Communicator *communicator, boost::python::list doubles)
    {
        return send_values<double, boost::python::list>(communicator, doubles);
    }
    bool send_double(Communicator *communicator, double _double)
    {
        return send_val<double>(communicator, _double);
    }
    bool send_string(Communicator *communicator, boost::python::str string)
    {
        return send_values<char, boost::python::str>(communicator, string);
    }
    bool send_bools(Communicator *communicator, boost::python::list bools)
    {
        return send_values<bool, boost::python::list>(communicator, bools);
    }
    bool send_bool(Communicator *communicator, bool _bool)
    {
        return send_val<bool>(communicator, _bool);
    }
    boost::python::list receive_ints(Communicator *communicator)
    {
        return receive_values<int>(communicator);
    }
    int receive_int(Communicator *communicator)
    {
        return receive_val<int>(communicator);
    }
    boost::python::list receive_doubles(Communicator *communicator)
    {
        return receive_values<double>(communicator);
    }
    double receive_double(Communicator *communicator)
    {
        return receive_val<double>(communicator);
    }
    boost::python::str receive_string(Communicator *communicator)
    {
        return boost::python::str("").join(receive_values<char>(communicator));
    }
    boost::python::list receive_bools(Communicator *communicator)
    {
        return receive_values<bool>(communicator);
    }
    bool receive_bool(Communicator *communicator)
    {
        return receive_val<bool>(communicator);
    }
}; // namespace communicate_export

void export_communicate()
{
    using namespace communicate_export;
    using namespace boost::python;

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
    def("receive_int", receive_int);
    def("send_ints", send_ints);
    def("send_int", send_int);

    // doubles
    def("receive_doubles", receive_doubles);
    def("receive_double", receive_double);
    def("send_doubles", send_doubles);
    def("send_double", send_double);

    // strings
    def("receive_string", receive_string);
    def("send_string", send_string);

    // booleans
    def("receive_bools", receive_bools);
    def("receive_bool", receive_bool);
    def("send_bools", send_bools);
    def("send_bool", send_bool);
}