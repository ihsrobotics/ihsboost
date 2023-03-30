/**
 * @file communicator.hpp
 * @author Eliot Hall
 * @brief Abstract communication class
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef IHSBOOST_COMMUNICATOR_HPP
#define IHSBOOST_COMMUNICATOR_HPP

#include "message_buf.hpp"

/**
 * @brief Abstract class for all communicators
 *
 */
class Communicator
{
public:
    /**
     * @brief Destroy the Communicator object
     * @details This has no effect in the base class
     *
     */
    virtual ~Communicator(){};

    /**
     * @brief Opens the communicator
     * @details This should be called automatically in the constructor
     *
     */
    virtual void open() = 0;

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close() = 0;

    /**
     * @brief Send a message
     *
     * @param message the message to send
     */
    virtual void send_msg(MessageBuf &&message);

    /**
     * @brief Send a message
     *
     * @param message the message to send
     */
    virtual void send_msg(MessageBuf &message);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     *
     * @return MessageBuf - the message that was received
     */
    virtual MessageBuf receive_msg() = 0;

    /**
     * @brief Create a MessageBuf object storing the given value
     *
     * @tparam T the type of the value
     * @param val the value to store
     * @return MessageBuf - a MessageBuf object that can be sent using communicators
     */
    template <typename T>
    MessageBuf create_msg(T val)
    {
        MessageBuf m(max_msg_size);
        m.set_val<T>(val);
        return m;
    }

    /**
     * @brief Create a MessageBuf object storing the given values
     *
     * @tparam T the type of the values
     * @param val a pointer to the values to store
     * @param len how many items to store
     * @return MessageBuf - a MessageBuf object that can be sent using communicators
     */
    template <typename T>
    MessageBuf create_msg(const T *val, uint16_t len)
    {
        MessageBuf m(max_msg_size);
        m.set_val<T>(val, len);
        return m;
    }

protected:
    /**
     * @brief Construct a new Communicator object
     *
     */
    Communicator();

    /**
     * @brief Construct a new Communicator object with the given
     * max_msg_size
     *
     * @param max_msg_size The maximum size of your messages
     */
    Communicator(uint32_t max_msg_size);

    /**
     * @brief Check to see if an error occured
     *
     * @throws CommunicationException - if retval is -1
     * @param retval the returned value; if it is -1, then an error has occurred
     * @param where where the error did/didn't occurr
     */
    void check_error(ssize_t retval, const char *where);

    /**
     * @brief Send the bytes of the MessageBuf over the communicator
     * @warning bytes will be deleted
     *
     * @param bytes the bytes, created from MessageBuf.to_bytes
     */
    virtual void send_bytes(char *bytes) = 0;

    uint32_t max_msg_size; ///< the maximum size of the message
};

#endif
/**@}*/