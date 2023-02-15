/**
 * @file sysvcommunicator.hpp
 * @author Eliot Hall
 * @brief SysV based communication
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef SYS_V_COMMUNICATOR_HPP
#define SYS_V_COMMUNICATOR_HPP

#include "communicator.hpp"
#include <sys/msg.h>

/**
 * @brief Class to communicate using System V message queues
 *
 */
class SysVCommunicator : public Communicator
{
public:
    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue, both `path`
     * and `identifier` must be the same on both communicators
     *
     * @param path a path to real file
     * @param identifier an integer to use as the "session id"
     */
    SysVCommunicator(const char *path, int identifier);

    /**
     * @brief Construct a new Sys V Communicator object
     *
     * @param path a path to real file
     * @param identifier an integer to use as the "session id"
     * @param max_msg_size the maximum size of your messages
     */
    SysVCommunicator(const char *path, int identifier, uint32_t max_msg_size);

    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue,
     * the provided key on both communicators must be the same
     *
     * @param key
     */
    SysVCommunicator(int key);

    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue,
     * the provided key on both communicators must be the same
     *
     * @param key
     * @param max_msg_size the maximum size of your messages
     */
    SysVCommunicator(int key, uint32_t max_msg_size);

    /**
     * @brief Destroy and close the Sys V Communicator object
     *
     */
    virtual ~SysVCommunicator();

    /**
     * @brief Opens the communicator
     * @details This should be called automatically in the constructor
     *
     */
    virtual void open();

    /**
     * @brief Close the communicator.
     * @details This is called automatically in the deconstructor.
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
    key_t k;
    int msg_q_id;
};

#endif
/**@}*/