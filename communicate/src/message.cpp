#include "message.hpp"

Message::Message(std::string msg) : length(msg.size())
{
    memcpy(buf, msg.c_str(), length * sizeof(char));
}

std::string Message::get_msg()
{
    std::string ret = "";
    for (size_t i = 0; i < length; ++i)
    {
        ret.push_back(buf[i]);
    }
    return ret;
}