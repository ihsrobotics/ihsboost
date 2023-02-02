#ifndef COMMUNICATION_EXCEPTION_HPP
#define COMMUNICATION_EXCEPTION_HPP

#include <exception>
#include <string>

class CommunicationException : public std::exception
{
public:
    CommunicationException(std::string where, int error_code);

    int get_error_code();

    virtual const char *what();

private:
    int error_code;
    std::string where;
};

#endif