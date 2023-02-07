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
     * @param identifier a character to use as the "session id"
     */
    SysVCommunicator(const char *path, char identifier);
    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue,
     * the provided key on both communicators must be the same
     *
     * @param key
     */
    SysVCommunicator(int key);
    virtual ~SysVCommunicator();

    virtual void send_msg(std::string message);

    virtual std::string receive_msg();

    virtual void close();

private:
    key_t k;
    int msg_q_id;
};

#endif