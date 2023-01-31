#include <iostream>
#include <sstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "message.hpp"
#include "communicator.hpp"

using namespace std;
using namespace chrono;

class SocketServer : public Communicator
{
public:
    SocketServer(int port)
    {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        check_error(server_fd, "creating host socket");
        cout << "server fd is " << server_fd << endl;

        // set socket options
        int optval = 1;
        int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval));
        check_error(ret, "setting socket options");

        // set address
        sockaddr_in addr;
        addr.sin_family = AF_INET; // IPv4
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        int addrlen = sizeof(addr);

        // bind
        ret = bind(server_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
        check_error(ret, "binding socket");

        // listen (allow connections)
        ret = listen(server_fd, 1); // listen for up to 1 connection

        // wait for connection (blocks)
        cout << "waiting for connection" << endl;

        socket_fd = accept(server_fd, reinterpret_cast<struct sockaddr *>(&addr), reinterpret_cast<socklen_t *>(&addrlen));
        check_error(socket_fd, "accepting connection");
    }

    virtual void send_msg(string msg)
    {
        Message m(msg);
        send(socket_fd, reinterpret_cast<const void *>(&m), sizeof(Message), 0);
    }

    virtual string receive_msg()
    {
        Message m("");
        int ret = read(socket_fd, reinterpret_cast<void *>(&m), sizeof(m));
        return m.get_msg();
    }

    virtual void close()
    {
        // close connected socket
        ::close(socket_fd);

        // close listening port
        shutdown(server_fd, SHUT_RDWR);
    }

private:
    int server_fd;
    int socket_fd;
};

class SocketClient : Communicator
{
public:
    SocketClient(const char *ipv4_addr, int port)
    {
        // make the socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        check_error(server_fd, "creating socket");

        // get the ipv4 address in binary form
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        int ret = inet_pton(AF_INET, ipv4_addr, &serv_addr.sin_addr);
        check_error(ret, "making address");

        // connect
        client_fd = connect(server_fd, reinterpret_cast<const sockaddr *>(&serv_addr), sizeof(serv_addr));
        check_error(client_fd, "connecting");
    }
    ~SocketClient()
    {
        close();
    }

    virtual void send_msg(string msg)
    {
        Message m(msg);
        send(server_fd, reinterpret_cast<const void *>(&m), sizeof(Message), 0);
    }

    virtual string receive_msg()
    {
        Message m("");
        int ret = read(server_fd, reinterpret_cast<void *>(&m), sizeof(m));
        return m.get_msg();
    }

    virtual void close()
    {
        ::close(client_fd);
    }

private:
    int server_fd;
    int client_fd;
};

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

    if (string(argv[1]) == "listener")
    {
        cout << "listener" << endl;
        SocketClient listener("127.0.0.1", port);

        while (1)
        {
            cout << listener.receive_msg() << endl;
        }
    }

    return 0;
}