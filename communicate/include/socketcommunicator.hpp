#ifndef SOCKET_COMMUNICATOR_HPP
#define SOCKET_COMMUNICATOR_HPP

#include <string>
#include "message.hpp"
#include "communicator.hpp"

class SocketServer : public Communicator
{
public:
    SocketServer(int port);
    virtual ~SocketServer();

    virtual void send_msg(std::string msg);

    virtual std::string receive_msg();

    virtual void close();

private:
    int server_fd;
    int socket_fd;
};

class SocketClient : Communicator
{
public:
    SocketClient(const char *ipv4_addr, int port);

    ~SocketClient();

    virtual void send_msg(std::string msg);

    virtual std::string receive_msg();

    virtual void close();

private:
    int server_fd;
    int client_fd;
};

#endif