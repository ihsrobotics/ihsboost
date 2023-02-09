#include "message.hpp"
#include <algorithm>
#include <memory.h>
#include <iostream>
#include <cstring>

using namespace std;

Message::MessageInfo::MessageInfo(uint32_t max_size) : length(0), max_size(max_size){};
Message::MessageInfo::MessageInfo(uint32_t length, uint32_t max_size) : length(length), max_size(max_size){};

Message::Message(uint32_t max_size) : message_info(max_size), buf(nullptr){};

Message::Message(string &msg, uint32_t max_size) : Message(msg.c_str(), max_size){};

Message::Message(const char *msg, uint32_t max_size) : message_info(min<size_t>(strlen(msg), static_cast<size_t>(max_size)), max_size), buf(new char[max_size])
{
    // clear buffer and copy over data onto it
    memset(reinterpret_cast<void *>(buf), 0, max_size * sizeof(char));
    memcpy(reinterpret_cast<void *>(buf), reinterpret_cast<const void *>(msg), message_info.length * sizeof(char));
}

Message::~Message()
{
    delete[] buf;
    buf = nullptr;
}

std::string Message::get_msg()
{
    std::string ret = "";
    for (uint32_t i = 0; i < message_info.length; ++i)
    {
        ret.push_back(buf[i]);
    }
    return ret;
}

uint32_t Message::get_num_bytes()
{
    return get_num_bytes(message_info.max_size);
}

uint32_t Message::get_num_bytes(uint32_t max_msg_size)
{
    return sizeof(Message::MessageInfo) + max_msg_size;
}

char *Message::to_bytes()
{
    char *ret = new char[get_num_bytes()];
    memcpy(reinterpret_cast<void *>(ret), reinterpret_cast<const void *>(&message_info), sizeof(Message::MessageInfo));
    memcpy(reinterpret_cast<void *>(ret + sizeof(Message::MessageInfo)), reinterpret_cast<const void *>(buf), message_info.max_size);
    return ret;
}

void Message::from_bytes(char *bytes, bool delete_bytes)
{
    // copy the Message Info object
    memcpy(reinterpret_cast<void *>(&message_info), reinterpret_cast<const void *>(bytes), sizeof(Message::MessageInfo));

    // clear whatever was in our buf, then resize it
    if (buf != nullptr)
    {
        delete[] buf;
    }
    buf = new char[message_info.max_size];

    // copy the buffer
    memcpy(reinterpret_cast<void *>(buf), reinterpret_cast<const void *>(bytes + sizeof(Message::MessageInfo)), message_info.max_size);

    // delete the bytes if told to
    if (delete_bytes)
    {
        delete[] bytes;
        bytes = nullptr;
    }
}