#include "message.hpp"
#include <algorithm>
#include <memory.h>
#include <iostream>
#include <cstring>

using namespace std;

Message::Message(uint32_t max_size) : max_size(max_size), length(0), buf(nullptr){};

Message::Message(string &msg, uint32_t max_size) : Message(msg.c_str(), max_size){};

Message::Message(const char *msg, uint32_t max_size) : max_size(max_size), length(min<size_t>(strlen(msg), static_cast<size_t>(max_size))), buf(new char[max_size])
{
    // clear buffer and copy over data onto it
    memset(reinterpret_cast<void *>(buf), 0, max_size * sizeof(char));
    memcpy(reinterpret_cast<void *>(buf), reinterpret_cast<const void *>(msg), (length + 1) * sizeof(char)); // length +1 to include the \0
}

Message::~Message()
{
    delete[] buf;
    buf = nullptr;
}

std::string Message::get_msg()
{
    std::string ret = "";
    for (uint32_t i = 0; i < length; ++i)
    {
        ret.push_back(buf[i]);
    }
    return ret;
}

uint32_t Message::get_num_bytes()
{
    return get_num_bytes(max_size);
}

uint32_t Message::get_num_bytes(uint32_t max_msg_size)
{
    return sizeof(Message) + max_msg_size;
}

char *Message::to_bytes()
{
    char *ret = new char[get_num_bytes()];
    memcpy(reinterpret_cast<void *>(ret), reinterpret_cast<const void *>(this), sizeof(Message));
    memcpy(reinterpret_cast<void *>(ret + sizeof(Message)), reinterpret_cast<const void *>(buf), max_size);
    return ret;
}

void Message::from_bytes(char *bytes, bool delete_bytes)
{
    // store our pointer since it gets overridden in memcpy
    // if didn't do this, we'd be leaving a dangling memory segment
    char *old_pointer = buf;

    // copy the Message object
    memcpy(reinterpret_cast<void *>(this), reinterpret_cast<const void *>(bytes), sizeof(Message));

    // set our buf back to our memory segment
    buf = old_pointer;

    // clear whatever was in our buf, then resize it
    delete[] buf;
    buf = new char[max_size];

    // copy the buffer
    memcpy(reinterpret_cast<void *>(buf), reinterpret_cast<const void *>(bytes + sizeof(Message)), max_size);

    // delete the bytes if told to
    if (delete_bytes)
    {
        delete[] bytes;
        bytes = nullptr;
    }
}