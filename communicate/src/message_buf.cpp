#include "message_buf.hpp"

MessageBuf::MessageBuf() : attrs(), data_holder(nullptr){};
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
            delete reinterpret_cast<DataHolder<void *> *>(data_holder);
        }
        data_holder = nullptr;
    }

    // clean the other things
    attrs.reset();
}

const std::type_info &MessageBuf::get_type() const
{
    // check if empty
    if (is_empty())
    {
        throw EmptyBufException();
    }

    return *reinterpret_cast<const std::type_info *>(attrs.tp_info);
}

bool MessageBuf::is_empty() const
{
    return attrs.empty;
}

uint32_t MessageBuf::get_size() const
{
    return sizeof(BufAttrs) + attrs.data_holder_size;
}

char *MessageBuf::to_bytes() const
{
    char *ret = new char[get_size()];

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
MessageBuf::BufAttrs::BufAttrs() : empty(true), was_from_bytes(false), tp_info(), data_holder_size(0){};
void MessageBuf::BufAttrs::reset()
{
    empty = true;
    data_holder_size = 0;
    was_from_bytes = false;
}