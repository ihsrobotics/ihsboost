#include "communication_exception.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <memory.h>
using namespace std;

CommunicationException::CommunicationException(const char *where, int error_code) : where(where), msg(nullptr), error_code(error_code)
{
    ostringstream o;

    o << "error while " << where << " with error " << error_code << endl;
    string temp = o.str();
    msg = new char[temp.size() + 1];
    memcpy(reinterpret_cast<void *>(msg), reinterpret_cast<const void *>(temp.c_str()), temp.size() + 1);
};
CommunicationException::CommunicationException(CommunicationException &c) : CommunicationException(c.where, c.error_code){};

CommunicationException::~CommunicationException()
{
    delete[] msg;
}

int CommunicationException::get_error_code() const { return error_code; }

const char *CommunicationException::what() const noexcept
{
    return static_cast<const char *>(msg);
}