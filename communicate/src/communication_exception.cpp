#include "communication_exception.hpp"
#include <string>
#include <sstream>
using namespace std;

CommunicationException::CommunicationException(string where, int error_code) : where(where), error_code(error_code){};

int CommunicationException::get_error_code() { return error_code; }

const char *CommunicationException::what()
{
    std::ostringstream o;
    o << "error while " << where << " with error " << error_code << endl;
    return o.str().c_str();
}