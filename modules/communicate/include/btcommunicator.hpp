/**
 * @file btcommunicator.hpp
 * @author Eliot Hall
 * @brief Bluetooth-based communication
 * @version 0.1
 * @date 2023-03-30
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IHSBOOST_BT_COMMUNICATOR_HPP
#define IHSBOOST_BT_COMMUNICATOR_HPP

#include <string>
#include "communicator.hpp"

/**
 * @brief Server class for communicating over bluetooth
 *
 */
class BluetoothServer : public Communicator
{
public:
    /**
     * @brief Construct a new Bluetooth Server object
     *
     * @param max_msg_size The maximum size of your messages
     */
    BluetoothServer(uint32_t max_msg_size);

    /**
     * @brief Construct a new Bluetooth Server object
     *
     */
    BluetoothServer();

    /**
     * @brief Destroy the Bluetooth Server object
     *
     */
    virtual ~BluetoothServer();

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
     * @return MessageBuf - the message that was received
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

    int bt_socket; ///< server fd
    int client_fd; ///< client fd on which to send and receive data
};

/**
 * @brief Client class for communicating over bluetooth
 *
 */
class BluetoothClient : public Communicator
{
public:
    /**
     * @brief Construct a new Bluetooth Client object
     *
     * @param target_addr the bluetooth address to connect to
     * @param max_msg_size the maximum size of your messages
     */
    BluetoothClient(std::string target_addr, uint32_t max_msg_size);

    /**
     * @brief Construct a new Bluetooth Client object
     *
     * @param target_addr the bluetooth address to connect to
     */
    BluetoothClient(std::string target_addr);

    /**
     * @brief Destroy the Bluetooth Client object
     *
     */
    virtual ~BluetoothClient();

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
     * @return MessageBuf - the message that was received
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

    std::string target_addr; ///< the address to connect to
    int bt_socket;           ///< the socket fd on which to send and receive data
};

#endif