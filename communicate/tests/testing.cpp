#include <ihsboost/all.hpp>
#include <iostream>
#include <sstream>
#include <thread>
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
             << "or `SocketCommunicator`" << endl;
        return -1;
    }

    int num_times = 15;

    // sysv
    int id = 33;
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

    if (string(argv[1]) == "talker")
    {
        cout << "talker" << endl;

        int count = 0;
        ostringstream s;
        for (int i = 0; i < num_times; ++i)
        {
            s << "message " << count;

            cout << "sending message : " << s.str() << endl;

            c->send_msg(s.str());
            this_thread::sleep_for(milliseconds(500));

            s.str("");
            s.clear();
            ++count;
        }
    }

    if (string(argv[1]) == "listener")
    {
        cout << "listener" << endl;

        for (int i = 0; i < num_times; ++i)
        {
            cout << c->receive_msg() << endl;
        }
    }
    delete c;

    return 0;
}