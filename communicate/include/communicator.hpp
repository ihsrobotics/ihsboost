#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP
#include <string>

class Communicator
{
public:
    virtual ~Communicator(){};

    /**
     * @brief Send a message
     *
     * @param message the message to send
     */
    virtual void send_msg(std::string message) = 0;

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return std::string - the message that was received
     */
    virtual std::string receive_msg() = 0;

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close() = 0;

protected:
    void check_error(int retval, const char *where);
};

#endif