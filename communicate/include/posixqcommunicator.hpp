/**
 * @file posixqcommunicator.hpp
 * @author Eliot Hall
 * @brief Posix-based mqueue communication
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef POSIX_Q_COMMUNICATOR_HPP
#define POSIX_Q_COMMUNICATOR_HPP

#include "communicator.hpp"
#include <string>
#include <mqueue.h>

/**
 * @brief Class for communicating over posix-based mqueue's
 *
 */
class PosixQCommunicator : public Communicator
{
public:
    /**
     * @brief Construct a new Posix Q Communicator object
     *
     * @param name name of the message queue. Make sure that it starts with `/`
     * @param max_msgs the maximum number of messages that the posix q
     * will be able to hold (defaults to 10)
     */
    PosixQCommunicator(const char *name, size_t max_msgs = 10);

    /**
     * @brief Construct a new Posix Q Communicator object
     * Make sure to preface your name with `/`
     *
     * @param name name of the message queue. Make sure that it starts with `/`
     * @param max_msgs the maximum number of messages that the posix q
     * will be able to hold
     * @param max_msg_size The maximum size of your messages
     */
    PosixQCommunicator(const char *name, size_t max_msgs, uint32_t max_msg_size);

    /**
     * @brief Destroy the Posix Q Communicator object
     * @details closes and attempts to unlink the message queue if it hasn't
     * already been unlinked
     *
     */
    virtual ~PosixQCommunicator();

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

    mqd_t msg_q_id;
    char *_name;
    size_t max_msgs;
};

#endif
/**@}*/