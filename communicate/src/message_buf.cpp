#include "message_buf.hpp"
#include <memory.h>
#include <iostream>

MessageBuf::MessageBuf(uint32_t buf_size) : attrs(buf_size), data_holder(nullptr){};
MessageBuf::MessageBuf(MessageBuf &&other)
{
    // case where other is this
    if (this == &other)
    {
        return;
    }

    // steal attrs
    attrs = other.attrs;
    data_holder = other.data_holder;

    // set the other one to nullptr so
    // that its cleanup doesn't mess with us
    other.data_holder = nullptr;
}

MessageBuf::~MessageBuf()
{
    reset();
}

void MessageBuf::reset()
{
    // only delete data if it was allocated
    if (!attrs.empty)
    {
        // use the correct delete
        // depends on whether it was constructed from bytes or just a new
        if (attrs.was_from_bytes)
        {
            delete[] reinterpret_cast<char *>(data_holder);
        }
        else
        {
            delete data_holder;
        }
        data_holder = nullptr;
    }

    // clean the other things
    attrs.reset();
}

const std::uint64_t MessageBuf::get_type_hash() const
{
    // check if empty
    if (is_empty())
    {
        throw EmptyBufException();
    }

    return attrs.tp_hash;
}

bool MessageBuf::is_empty() const
{
    return attrs.empty;
}

uint32_t MessageBuf::get_buffered_size() const
{
    return get_size(attrs.buf_size);
}

uint32_t MessageBuf::get_true_size() const
{
    return get_size(attrs.data_holder_size);
}

uint16_t MessageBuf::get_length() const
{
    return attrs.data_holder_len;
}

char *MessageBuf::to_bytes() const
{
    char *ret = new char[get_buffered_size()];
    memset(reinterpret_cast<void *>(ret), 0, get_buffered_size());

    // copy data attributes
    memcpy(reinterpret_cast<void *>(ret), reinterpret_cast<const void *>(&attrs), sizeof(BufAttrs));

    // copy data if any
    if (!is_empty())
    {
        memcpy(reinterpret_cast<void *>(ret + sizeof(BufAttrs)), reinterpret_cast<const void *>(data_holder), attrs.data_holder_size);
    }

    // return buffer
    return ret;
}

void MessageBuf::from_bytes(char *bytes, bool delete_bytes)
{
    // clean anything we have
    reset();

    // get attrs
    memcpy(reinterpret_cast<void *>(&attrs), reinterpret_cast<const void *>(bytes), sizeof(BufAttrs));

    // get data if any
    if (!is_empty())
    {
        char *temp = new char[attrs.data_holder_size];
        memcpy(reinterpret_cast<void *>(temp), reinterpret_cast<const void *>(bytes + sizeof(BufAttrs)), attrs.data_holder_size);
        data_holder = temp;
        attrs.was_from_bytes = true;
    }

    // cleanup bytes if necessary
    if (delete_bytes)
    {
        delete[] bytes;
        bytes = nullptr;
    }
}

// MessageBuf::BufAttrs stuf
MessageBuf::BufAttrs::BufAttrs() : empty(true), was_from_bytes(false), tp_hash(0), data_holder_size(0), data_holder_len(0), buf_size(0){};
MessageBuf::BufAttrs::BufAttrs(uint32_t buf_size) : empty(true), was_from_bytes(false), tp_hash(0), data_holder_size(0), data_holder_len(0), buf_size(buf_size){};
void MessageBuf::BufAttrs::reset()
{
    empty = true;
    was_from_bytes = false;

    data_holder_size = 0;
    data_holder_len = 0;

    tp_hash = 0;
}