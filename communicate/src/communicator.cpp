#include "communicator.hpp"
#include "message.hpp"
#include "communication_exception.hpp"

Communicator::Communicator() : max_msg_size(MAX_MSG_SIZE){};
Communicator::Communicator(uint32_t max_msg_size) : max_msg_size(max_msg_size){};

void Communicator::check_error(int retval, const char *where)
{
    if (retval == -1)
    {
        CommunicationException error(where, errno);
        throw error;
    }
}