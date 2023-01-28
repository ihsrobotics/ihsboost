#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <memory.h>

#define MAX_MSG_SIZE 200
class Message
{
public:
    Message(){};
    Message(std::string msg) : length(msg.size())
    {
        memcpy(buf, msg.c_str(), length * sizeof(char));
    }
    std::string get_msg()
    {
        std::string ret = "";
        for (size_t i = 0; i < length; ++i)
        {
            ret.push_back(buf[i]);
        }
        return ret;
    }

private:
    size_t length;
    char buf[MAX_MSG_SIZE];
};
#endif