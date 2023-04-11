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
#ifndef IHSBOOST_SYS_V_COMMUNICATOR_HPP
#define IHSBOOST_SYS_V_COMMUNICATOR_HPP

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

    /**
     * @brief Check if the sysv file already exists
     *
     * @return true
     * @return false
     */
    bool check_exists();

    key_t k;      ///< the key for the sysv file
    bool existed; ///< if the sysv file existed prior to this object
    int msg_q_id; ///< the id of the attached sysv file
};

#endif
/**@}*/