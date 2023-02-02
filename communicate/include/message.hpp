#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <memory.h>

// This is the maximum number of characters in a message.
// note that it it should never exceed 256.
#define MAX_MSG_SIZE 200

class Message
{
public:
    /**
     * @brief Construct a new Message object with no message
     *
     */
    Message();

    /**
     * @brief Construct a new Message object with the provided message
     *
     * @param msg the message that this Message object will hold
     */
    Message(std::string msg);

    /**
     * @brief Get the stored message as a string
     *
     * @return std::string - the message that this Message contains
     */
    std::string get_msg();

private:
    uint8_t length;
    char buf[MAX_MSG_SIZE];
};
#endif