#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP
#include <string>

class Communicator
{
public:
    Communicator();

    Communicator(uint32_t max_msg_size);

    virtual ~Communicator(){};

    /**
     * @brief Opens the communicator
     * @details This should be called automatically in the constructor
     *
     */
    virtual void open() = 0;

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close() = 0;

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

protected:
    void check_error(int retval, const char *where);
    uint32_t max_msg_size;
};

#endif