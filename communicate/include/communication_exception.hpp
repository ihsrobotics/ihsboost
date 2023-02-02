#ifndef COMMUNICATION_EXCEPTION_HPP
#define COMMUNICATION_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class CommunicationException : public std::exception
{
public:
    CommunicationException(const char *where, int error_code);
    CommunicationException(CommunicationException &c);
    ~CommunicationException();

    int get_error_code() const;

    virtual const char *what() const noexcept override;

private:
    int error_code;
    const char *where;
    char *msg;
};

#endif