#include <iostream>
#include "test.hpp"
#include <chrono>
#include <thread>
#include <ihsboost/communicate.hpp>
using namespace std;
using namespace chrono;

int main(int argc, const char *argv[])
{
    // exit if not provided communicator
    if (argc != 2)
    {
        cerr << "no communicator provided" << endl;
        cerr << "Valid communicators: PosixQCommunicator, SocketCommunicator, SysVCommunicator" << endl;
        return 1;
    }

    // run the python
    string command = "python3 ./back_and_forth_test.py ";
    command += string(argv[1]) + " &";
    int start_flag = system(command.c_str());
    assert_equals(start_flag, 0, "starting process");

    // initialize communicator
    Communicator *com = nullptr;
    if (string(argv[1]) == string("PosixQCommunicator"))
    {
        com = new PosixQCommunicator("/back_and_forth_queue");
    }
    else if (string(argv[1]) == string("SocketCommunicator"))
    {
        com = new SocketServer(2222);
    }
    else if (string(argv[1]) == string("SysVCommunicator"))
    {
        com = new SysVCommunicator(22);
    }
    else
    {
        cerr << "invalid communicator" << endl;
        return 2;
    }

    // receive ready flag from python
    assert_equals(com->receive_msg().get_val<int>(), 1, "receiving flag");

    // send some values
    int val = 257;
    com->send_msg(com->create_msg<int>(val));
    this_thread::sleep_for(milliseconds(10));

    // check that we received the processed value
    assert_equals(com->receive_msg().get_val<int>(), 23 * val, "getting it back");

    // send strings over
    string start = "hello";
    string expected = "hello world";
    com->send_msg(com->create_msg<char>(start.c_str(), static_cast<uint16_t>(start.size()))); // dont need a +1 when sending to python
    this_thread::sleep_for(milliseconds(200));
    assert_equals(string(com->receive_msg().get_ptr_val<char>()), expected, "getting strings from python");

    return 0;
}