#ifndef SOCKET_COMMUNICATOR_HPP
#define SOCKET_COMMUNICATOR_HPP

#include <string>
#include "message.hpp"
#include "communicator.hpp"

class SocketServer : public Communicator
{
public:
    /**
     * @brief Construct a new Socket Server object
     *
     * @param port the port to host the server on
     */
    SocketServer(int port);
    SocketServer(int port, uint32_t max_msg_size);
    virtual ~SocketServer();

    /**
     * @brief Opens the communicator
     * @details This should be called automatically in the constructor
     *
     */
    virtual void open();

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close();

    /**
     * @brief Send a message
     *
     * @param message the message to send
     */
    virtual void send_msg(std::string msg);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual std::string receive_msg();

private:
    int server_fd;
    int socket_fd;
    int port;
};

class SocketClient : public Communicator
{
public:
    /**
     * @brief Construct a new Socket Client object
     *
     * @param ipv4_addr The ip address to connect to
     * @param port the port to connect to
     */
    SocketClient(const char *ipv4_addr, int port);
    SocketClient(const char *ipv4_addr, int port, uint32_t max_msg_size);

    ~SocketClient();

    /**
     * @brief Send a message
     *
     * @param message the message to send
     */
    virtual void send_msg(std::string msg);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual std::string receive_msg();

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close();

    virtual void open();

private:
    int server_fd;
    int client_fd;
    int port;
    const char *ipv4_addr;
};

#endif