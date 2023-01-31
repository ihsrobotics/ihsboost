#include "socketcommunicator.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

SocketServer::SocketServer(int port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_error(server_fd, "creating host socket");

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
    cout << "waiting for connection ..." << endl;

    socket_fd = accept(server_fd, reinterpret_cast<struct sockaddr *>(&addr), reinterpret_cast<socklen_t *>(&addrlen));
    check_error(socket_fd, "accepting connection");
}
SocketServer::~SocketServer()
{
    close();
}

void SocketServer::send_msg(string msg)
{
    Message m(msg);
    int ret = send(socket_fd, reinterpret_cast<const void *>(&m), sizeof(Message), 0);
    check_error(ret, "sending message");
}

string SocketServer::receive_msg()
{
    Message m("");
    int ret = read(socket_fd, reinterpret_cast<void *>(&m), sizeof(Message));
    check_error(ret, "receiving message");
    return m.get_msg();
}

void SocketServer::close()
{
    // close connected socket
    ::close(socket_fd);

    // close listening port
    shutdown(server_fd, SHUT_RDWR);
}

SocketClient::SocketClient(const char *ipv4_addr, int port)
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

SocketClient::~SocketClient()
{
    close();
}

void SocketClient::send_msg(string msg)
{
    Message m(msg);
    int ret = send(server_fd, reinterpret_cast<const void *>(&m), sizeof(Message), 0);
    check_error(ret, "sending message");
}

string SocketClient::receive_msg()
{
    Message m("");
    int ret = read(server_fd, reinterpret_cast<void *>(&m), sizeof(Message));
    check_error(ret, "receiving message");
    return m.get_msg();
}

void SocketClient::close()
{
    ::close(client_fd);
}
