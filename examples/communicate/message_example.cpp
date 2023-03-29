/**
 * @file message_test.cpp
 * @author Eliot Hall
 * @brief This file, when compiled can act as both talker and listener
 * using any of the communicators within ihsboost
 * After compiling, you can run it.
 * Usage: `./a.out (talker/listener) (communicator)`
 * Example: `./a.out talker SysVCommunicator`
 * Example: `./a.out listener SysVCommunicator`
 * Valid communicators:
 * @li SysVCommunicator
 * @li PosixQCommunicator
 * @li SocketCommunicator
 * @li SHMCommunicator
 * @version 0.1
 * @date 2023-03-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <ihsboost/all.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        cerr << "need 2 arguments to be provided" << endl;
        cerr << "first should be either `talker` or `listener`" << endl;
        cerr << "second should be the communicator type: `SysVCommunicator`, `PosixQCommunicator`"
             << "`SocketCommunicator` or `SHMCommunicator`" << endl;
        return -1;
    }

    // how many messages to send
    int num_times = 15;

    // variables needed for communicators.
    // sysv
    int id = 345;
    // posix
    const char *name = "/my_queue";
    // socket
    int port = 8080;
    const char *ip = "127.0.0.1";

    // initialize communicator to correct type
    Communicator *c;
    if (string(argv[2]) == "SysVCommunicator")
    {
        c = new SysVCommunicator(id);
    }
    else if (string(argv[2]) == "PosixQCommunicator")
    {
        c = new PosixQCommunicator(name);
    }
    else if (string(argv[2]) == "SHMCommunicator")
    {
        c = new SHMCommunicator(id);
    }
    else if (string(argv[2]) == "SocketCommunicator")
    {
        if (string(argv[1]) == "talker")
        {
            c = new SocketServer(port);
        }
        else
        {
            c = new SocketClient(ip, port);
        }
    }
    else
    {
        cerr << "invalid talker provided" << endl;
        return 1;
    }

    // run the main code
    if (string(argv[1]) == "talker")
    {
        cout << "talker" << endl;

        int count = 0;
        ostringstream o;
        for (int i = 0; i < num_times; ++i)
        {
            o << "cool " << i << endl;
            string ret = o.str();
            cout << "sending message : " << ret << endl;
            c->send_msg(c->create_msg<char>(ret.c_str(), ret.size())); // send a "string" (it's really a list of char's)

            // send list of doubles instead
            // double my_double[3] = {3.14, 2.22, 7.77};
            // for (int i = 0; i < 3; ++i)
            // {
            //     cout << my_double[i] << endl;
            // }
            // cout << endl;
            // c->send_msg(c->create_msg<double>(my_double, 3));

            this_thread::sleep_for(milliseconds(500));

            o.str("");
            o.clear();
            ++count;
        }
    }
    if (string(argv[1]) == "listener")
    {
        cout << "listener" << endl;

        for (int i = 0; i < num_times; ++i)
        {
            // receive list of doubles
            // MessageBuf ret = c->receive_msg();
            // double *vals = ret.get_ptr_val<double>();
            // for (int i = 0; i < 3; ++i)
            // {
            //    cout << vals[i] << endl;
            //}
            // cout << endl;

            // receive char* message (basically a string)
            cout << c->receive_msg().get_ptr_val<char>() << endl;
        }
    }
    delete c;

    return 0;
}