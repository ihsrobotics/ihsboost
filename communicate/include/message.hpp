#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <memory.h>

#define MAX_MSG_SIZE 200
class Message
{
public:
    Message(){};
    Message(std::string msg);
    std::string get_msg();

private:
    size_t length;
    char buf[MAX_MSG_SIZE];
};
#endif