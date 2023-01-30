#ifndef POSIX_Q_COMMUNICATOR_HPP
#define POSIX_Q_COMMUNICATOR_HPP

#include "message.hpp"
#include "communicator.hpp"
#include <string>
#include <mqueue.h>

class PosixQCommunicator : public Communicator
{
public:
    /**
     * @brief Construct a new Posix Q Communicator object
     * Make sure to preface your name with `/`
     *
     * @param name name of the message queue. Make sure that it starts
     * with `/`
     */
    PosixQCommunicator(const char *name, size_t max_msgs = 10);
    ~PosixQCommunicator();

    virtual void send_msg(std::string message);
    virtual std::string receive_msg();
    virtual void close();

private:
    mqd_t msg_q_id;
    const char *_name;
};

#endif