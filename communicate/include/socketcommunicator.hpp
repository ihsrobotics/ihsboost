/**
 * @file socketcommunicator.hpp
 * @author Eliot Hall
 * @brief Socket based communication
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef SOCKET_COMMUNICATOR_HPP
#define SOCKET_COMMUNICATOR_HPP

#include "communicator.hpp"

/**
 * @brief Server Class for communicating over sockets
 *
 */
class SocketServer : public Communicator
{
public:
    /**
     * @brief Construct a new Socket Server object
     *
     * @param port the port to host the server on
     */
    SocketServer(int port);

    /**
     * @brief Construct a new Socket Server object
     *
     * @param port the port to host the server on
     * @param max_msg_size The maximum size of your messages
     */
    SocketServer(int port, uint32_t max_msg_size);

    /**
     * @brief Destroy the Socket Server object
     *
     */
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
    virtual void send_msg(MessageBuf message);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual MessageBuf receive_msg();

private:
    int server_fd; ///< the server field descriptor
    int socket_fd; ///< the field descriptor for the connected socket
    int port;      ///< the port to connect to
};

/**
 * @brief Client class for communicating over sockets
 *
 */
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

    /**
     * @brief Construct a new Socket Client object
     *
     * @param ipv4_addr The ip address to connect to
     * @param port the port to connect to
     * @param max_msg_size The maximum size of your messages
     */
    SocketClient(const char *ipv4_addr, int port, uint32_t max_msg_size);

    /**
     * @brief Close the Socket Client connection
     *
     */
    virtual ~SocketClient();

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
    virtual void send_msg(MessageBuf message);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual MessageBuf receive_msg();

private:
    int server_fd;         ///< the field descriptor of the server
    int client_fd;         ///< the field descriptor of this SocketClient
    int port;              ///< the port to connect to
    const char *ipv4_addr; ///< the ip address to connect to
};

#endif
/**@}*/