#include <iostream>
#include <sstream>
#include "socketcommunicator.hpp"
#include <chrono>
#include <thread>
#include "message.hpp"
#include "communicator.hpp"

using namespace std;
using namespace chrono;

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        cerr << "need a second argument to be provided" << endl;
        cerr << "either `server` or `client`" << endl;
        return -1;
    }

    int port = 8080;
    if (string(argv[1]) == "server")
    {
        cout << "server" << endl;
        SocketServer server(port);

        int count = 0;
        ostringstream s;
        while (1)
        {
            s << "message " << count;

            cout << "sending message : " << s.str() << endl;

            server.send_msg(s.str());
            std::this_thread::sleep_for(milliseconds(500));

            s.str("");
            s.clear();
            ++count;
        }
    }

    if (string(argv[1]) == "client")
    {
        cout << "client" << endl;
        SocketClient listener("127.0.0.1", port); // 127.0.0.1 == localhost

        while (1)
        {
            cout << "received message : " << listener.receive_msg() << endl;
        }
    }

    return 0;
}