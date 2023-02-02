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
    Message(){};
    Message(std::string msg);
    std::string get_msg();

private:
    uint8_t length;
    char buf[MAX_MSG_SIZE];
};
#endif