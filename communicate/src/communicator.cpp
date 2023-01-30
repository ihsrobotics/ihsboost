#include "communicator.hpp"
#include <iostream>

using namespace std;

void Communicator::check_error(int retval, const char *where)
{
    if (retval == -1)
    {
        cerr << "errored while " << where << " with error " << errno << endl;
        throw errno;
    }
}