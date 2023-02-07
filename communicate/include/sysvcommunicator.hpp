#ifndef SYS_V_COMMUNICATOR_HPP
#define SYS_V_COMMUNICATOR_HPP

#include "message.hpp"
#include "communicator.hpp"
#include <sys/msg.h>
#include <string>

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
    SysVCommunicator(const char *path, int identifier, uint32_t max_msg_size);
    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue,
     * the provided key on both communicators must be the same
     *
     * @param key
     */
    SysVCommunicator(int key);
    SysVCommunicator(int key, uint32_t max_msg_size);

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
    virtual void send_msg(std::string message);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual std::string receive_msg();

private:
    key_t k;
    int msg_q_id;
};

#endif