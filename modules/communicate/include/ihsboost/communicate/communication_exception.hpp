/**
 * @file communication_exception.hpp
 * @author Eliot Hall
 * @brief Exceptions that may be thrown while communicating
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef IHSBOOST_COMMUNICATION_EXCEPTION_HPP
#define IHSBOOST_COMMUNICATION_EXCEPTION_HPP

#include <stdexcept>
#include <string>

/**
 * @brief Exception class for when things go wrong with communicators
 *
 */
class CommunicationException : public std::exception
{
public:
    /**
     * @brief Construct a new Communication Exception object
     *
     * @param where where the exception occurred
     * @param error_code the error code associated with this exception
     */
    CommunicationException(const char *where, int error_code);

    /**
     * @brief Construct a new Communication Exception object
     *
     * @param c the exception to copy
     */
    CommunicationException(CommunicationException &c);

    /**
     * @brief Destroy the Communication Exception object
     *
     */
    ~CommunicationException();

    /**
     * @brief Get the error code
     *
     * @return int
     */
    int get_error_code() const;

    /**
     * @brief Get the exception's description in readable form
     *
     * @return const char*
     */
    virtual const char *what() const noexcept override;

private:
    const char *where; ///! where it went wrong
    char *msg;         ///! msg to print
    int error_code;    ///! the error code that went wrong
};

/**
 * @brief Exception class to protect from bad reinterpret_cast's
 *
 */
class BadBufCastException : public std::bad_cast
{
public:
    /**
     * @brief Get the exception's description in readable form
     *
     * @return const char*
     */
    virtual const char *what() const noexcept override
    {
        return "get_val tried to return the wrong type!";
    }
};

/**
 * @brief Exception class to protect from nullptr segmentation faults
 *
 */
class EmptyBufException : public std::bad_exception
{
public:
    /**
     * @brief Get the exception's description in readable form
     *
     * @return const char*
     */
    virtual const char *what() const noexcept override
    {
        return "get_val was called but the buffer is empty!";
    }
};

#endif
/**@}*/