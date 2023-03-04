#include "socketcommunicator.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

using namespace std;

SocketServer::SocketServer(int port, uint32_t max_msg_size) : Communicator(max_msg_size), port(port)
{
    open();
}
SocketServer::SocketServer(int port) : Communicator(), port(port)
{
    open();
}
SocketServer::~SocketServer()
{
    close();
}

void SocketServer::open()
{
    // create socket
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

void SocketServer::close()
{
    cout << "closing SocketServer" << endl;
    // close connected socket
    ::close(socket_fd);

    // close listening port
    shutdown(server_fd, SHUT_RDWR);
}

void SocketServer::send_msg(MessageBuf message)
{
    char *bytes = message.to_bytes();
    int ret = send(socket_fd, reinterpret_cast<const void *>(bytes), MessageBuf::get_size(max_msg_size), 0);
    delete[] bytes;
    check_error(ret, "sending message");
}

MessageBuf SocketServer::receive_msg()
{
    // create bytes and message
    char *bytes = new char[MessageBuf::get_size(max_msg_size)];
    MessageBuf m(max_msg_size);

    // read into byte buffer
    int ret = read(socket_fd, reinterpret_cast<void *>(bytes), MessageBuf::get_size(max_msg_size));
    check_error(ret, "receiving message");

    // create message from bytes
    m.from_bytes(bytes);
    return m;
}

SocketClient::SocketClient(const char *ipv4_addr, int port, uint32_t max_msg_size) : Communicator(max_msg_size), port(port), ipv4_addr(ipv4_addr)
{
    open();
}
SocketClient::SocketClient(const char *ipv4_addr, int port) : Communicator(), port(port), ipv4_addr(ipv4_addr)
{
    open();
}

SocketClient::~SocketClient()
{
    close();
}

void SocketClient::open()
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

void SocketClient::close()
{
    cout << "closing SocketClient" << endl;
    ::close(client_fd);
}

void SocketClient::send_msg(MessageBuf message)
{
    char *bytes = message.to_bytes();
    int ret = send(server_fd, reinterpret_cast<const void *>(bytes), MessageBuf::get_size(max_msg_size), 0);
    delete[] bytes;
    check_error(ret, "sending message");
}

MessageBuf SocketClient::receive_msg()
{
    // get bytes and message
    char *bytes = new char[MessageBuf::get_size(max_msg_size)];
    MessageBuf m(max_msg_size);

    // read into bytes
    int ret = read(server_fd, reinterpret_cast<void *>(bytes), MessageBuf::get_size(max_msg_size));
    check_error(ret, "receiving message");

    // get message from bytes
    m.from_bytes(bytes);
    return m;
}
