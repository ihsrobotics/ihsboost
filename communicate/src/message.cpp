#include "message.hpp"

Message::Message(std::string msg) : length(static_cast<uint8_t>(msg.size()))
{
    memset(buf, 0, MAX_MSG_SIZE * sizeof(char));
    memcpy(buf, msg.c_str(), length * sizeof(char));
}

std::string Message::get_msg()
{
    std::string ret = "";
    for (uint8_t i = 0; i < length; ++i)
    {
        ret.push_back(buf[i]);
    }
    return ret;
}