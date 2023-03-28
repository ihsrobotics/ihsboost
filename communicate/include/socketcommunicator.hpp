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
#ifndef IHSBOOST_SOCKET_COMMUNICATOR_HPP
#define IHSBOOST_SOCKET_COMMUNICATOR_HPP

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
    SocketServer(uint16_t port);

    /**
     * @brief Construct a new Socket Server object
     *
     * @param port the port to host the server on
     * @param max_msg_size The maximum size of your messages
     */
    SocketServer(uint16_t port, uint32_t max_msg_size);

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
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual MessageBuf receive_msg();

private:
    /**
     * @brief Send the bytes of the MessageBuf over the communicator
     * @warning bytes will be deleted
     *
     * @param bytes the bytes, created from MessageBuf.to_bytes
     */
    virtual void send_bytes(char *bytes);

    int server_fd; ///< the server field descriptor
    int socket_fd; ///< the field descriptor for the connected socket
    uint16_t port; ///< the port to connect to
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
    SocketClient(std::string ipv4_addr, uint16_t port);

    /**
     * @brief Construct a new Socket Client object
     *
     * @param ipv4_addr The ip address to connect to
     * @param port the port to connect to
     * @param max_msg_size The maximum size of your messages
     */
    SocketClient(std::string ipv4_addr, uint16_t port, uint32_t max_msg_size);

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
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual MessageBuf receive_msg();

private:
    /**
     * @brief Send the bytes of the MessageBuf over the communicator
     * @warning bytes will be deleted
     *
     * @param bytes the bytes, created from MessageBuf.to_bytes
     */
    virtual void send_bytes(char *bytes);

    int server_fd;         ///< the field descriptor of the server
    int client_fd;         ///< the field descriptor of this SocketClient
    uint16_t port;         ///< the port to connect to
    std::string ipv4_addr; ///< the ip address to connect to
};

#endif
/**@}*/