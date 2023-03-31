#include "communicator.hpp"
#include "communication_exception.hpp"

Communicator::Communicator() : max_msg_size(DEFAULT_MAX_MSG_SIZE){};
Communicator::Communicator(uint32_t max_msg_size) : max_msg_size(max_msg_size){};

void Communicator::check_error(ssize_t retval, const char *where)
{
    if (retval == -1)
    {
        CommunicationException error(where, errno);
        perror(where);
        throw error;
    }
}

void Communicator::send_msg(MessageBuf &&message)
{
    send_bytes(message.to_bytes());
}
void Communicator::send_msg(MessageBuf &message)
{
    send_bytes(message.to_bytes());
}
