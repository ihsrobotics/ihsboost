#include "communicator.hpp"
#include "communication_exception.hpp"

void Communicator::check_error(int retval, const char *where)
{
    if (retval == -1)
    {
        CommunicationException error(where, errno);
        throw error;
    }
}