#include <iostream>
#include <ihsboost/communicate.hpp>
#include <thread>
#include <chrono>
#include <exception>
#include <vector>
#include <sstream>
using namespace std;
using namespace chrono;

class AssertionError : public logic_error
{
public:
    AssertionError(const char *what) : logic_error(what){};
};

template <typename T, typename V>
static void assert_equals(T desired, V given, string where = "main")
{
    if (desired != given)
    {
        ostringstream temp;
        temp << "Error in " << where << ": " << desired << " is not equal to " << given;
        throw AssertionError(temp.str().c_str());
    }
}
template <typename T, typename V>
static void assert_notequals(T not_desired, V given, string where = "main")
{
    if (not_desired == given)
    {
        ostringstream temp;
        temp << "Error in " << where << ": " << not_desired << " is equal to " << given << " (it shouldn't be)";
        throw AssertionError(temp.str().c_str());
    }
}

void send_ints(Communicator *c)
{
    // send ints
    for (int i = 0; i < 15; ++i)
    {
        c->send_msg(c->create_msg<int>(i));
        this_thread::sleep_for(milliseconds(50));
    }
}
void receive_ints(Communicator *c)
{
    for (int i = 0; i < 15; ++i)
    {
        assert_equals(c->receive_msg().get_val<int>(), i, "receive_ints");
    }
}
void test_ints(Communicator *sender, Communicator *receiver)
{
    thread send_thread(send_ints, sender);
    receive_ints(receiver);
    cout << "passed test ints" << endl;
    send_thread.join();
}

void send_doubles(Communicator *c, double multiplier)
{
    // send doubles
    for (int i = 0; i < 15; ++i)
    {
        c->send_msg(c->create_msg<double>(i * multiplier));
        this_thread::sleep_for(milliseconds(50));
    }
}
void receive_doubles(Communicator *c, double multiplier)
{
    for (int i = 0; i < 15; ++i)
    {
        assert_equals(c->receive_msg().get_val<double>(), i * multiplier, "receive doubles");
    }
}
void test_doubles(Communicator *sender, Communicator *receiver)
{
    double multiplier = 5.72;
    thread send_thread(send_doubles, sender, multiplier);
    receive_doubles(receiver, multiplier);
    cout << "passed test doubles" << endl;
    send_thread.join();
}

void send_strings(Communicator *c, vector<string> msgs)
{
    // send strings
    for (string msg : msgs)
    {
        c->send_msg(c->create_msg<char>(msg.c_str(), static_cast<uint16_t>(msg.size()) + 1));
        this_thread::sleep_for(milliseconds(50));
    }
}
void receive_strings(Communicator *c, vector<string> msgs)
{
    for (string msg : msgs)
    {
        assert_equals(string(c->receive_msg().get_ptr_val<char>()), msg, "receive strings");
    }
}
void test_strings(Communicator *sender, Communicator *receiver)
{
    vector<string> msgs = {"testa", "test b", "hello world", "looooooooong test"};
    thread send_thread(send_strings, sender, msgs);
    receive_strings(receiver, msgs);
    send_thread.join();
}

int main(int argc, const char *argv[])
{
    // exit if not provided communicator
    if (argc != 2)
    {
        cerr << "no communicator provided" << endl;
        cerr << "Valid communicators: PosixQCommunicator, SocketCommunicator, SysVCommunicator" << endl;
        return 1;
    }
    Communicator *com_a = nullptr;
    Communicator *com_b = nullptr;
    if (string(argv[1]) == string("PosixQCommunicator"))
    {
        com_a = new PosixQCommunicator("/test_queue");
        com_b = new PosixQCommunicator("/test_queue");
    }
    else if (string(argv[1]) == string("SocketCommunicator"))
    {
        thread make_socket([&com_a]() -> void
                           { com_a = new SocketServer(3060); });
        this_thread::sleep_for(milliseconds(1000)); // wait for thread to initialize
        com_b = new SocketClient("127.0.0.1", 3060);
        make_socket.join();
    }
    else if (string(argv[1]) == string("SysVCommunicator"))
    {
        com_a = new SysVCommunicator(33);
        com_b = new SysVCommunicator(33);
    }
    else
    {
        cerr << "invalid communicator" << endl;
        return 2;
    }

    // int tests
    test_ints(com_a, com_b);
    test_ints(com_b, com_a);

    // test doubles
    test_doubles(com_a, com_b);
    test_doubles(com_b, com_a);

    // test strings
    test_strings(com_a, com_b);
    test_strings(com_b, com_a);

    // cleanup
    delete com_a;
    delete com_b;
    return 0;
}