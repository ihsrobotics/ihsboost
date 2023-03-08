#include "communicate.hpp"
#include <boost/python.hpp>

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
}