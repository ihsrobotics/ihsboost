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

class BadBufCastException : public std::bad_cast
{
public:
    virtual const char *what() const noexcept override
    {
        return "get_val tried to return the wrong type!";
    }
};

class EmptyBufException : public std::bad_exception
{
public:
    virtual const char *what() const noexcept override
    {
        return "get_val was called but the buffer is empty!";
    }
};

#endif