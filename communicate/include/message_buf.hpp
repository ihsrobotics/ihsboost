/**
 * @file message_buf.hpp
 * @author Eliot Hall
 * @brief MessageBuf class for serializing and retreiving data
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef MESSAGE_BUF_HPP
#define MESSAGE_BUF_HPP
#include <typeinfo>
#include <memory.h>
#include "communication_exception.hpp"

#define DEFAULT_MAX_MSG_SIZE 100 ///< default maximum size for a MessageBuf's buffer

/**
 * @brief Class for storing data over communicators
 *
 */
class MessageBuf
{
public:
    // constructors and deconstructors
    /**
     * @brief Construct a new MessageBuf object
     *
     * @param buf_size how large the buffer size should be
     */
    MessageBuf(uint32_t buf_size);

    /**
     * @brief Move the other MessageBuf object to this MessageBuf
     *
     * @param other the MessageBuf to move to this
     */
    MessageBuf(MessageBuf &&other);

    /**
     * @brief Moves the other MessageBuf object to this MessageBuf
     *
     * @param other the MessageBuf to move to this
     * @return MessageBuf& *this
     */
    MessageBuf &operator=(MessageBuf &&other);

    /**
     * @brief Destroy the Message Buf object
     * @details deletes any data associated with it
     *
     */
    ~MessageBuf();

    /**
     * @brief Cleans up the MessageBuf, deleting any data if there was any
     * and resetting buffer attributes
     *
     */
    void reset();

    /**
     * @brief Get the type hash describing data held
     * @exception Will throw EmptyBufException if the buffer is empty
     *
     * @return const std::uint64_t the type hash
     */
    const std::uint64_t get_type_hash() const;

    /**
     * @brief Return whether or not the message buf is empty
     *
     * @return true if it is empty
     * @return false if it isn't empty
     */
    bool is_empty() const;

    /**
     * @brief Get the number of bytes that this current
     * object takes when converted to bytes
     *
     * @return uint32_t the number of bytes
     */
    uint32_t get_buffered_size() const;

    /**
     * @brief Get the true size of this object
     *
     * @return uint32_t
     */
    uint32_t get_true_size() const;

    /**
     * @brief Get how long the data is
     * @details value of 1 means that it only holds one of that type,
     * value of 2 means that there are 2, etc
     *
     * @return uint16_t how long the data is
     */
    uint16_t get_length() const;

    /**
     * @brief Get the minimum number of bytes that a MessageBuf
     * of type T will take that holds `len` number of T's
     * will take
     *
     * @tparam T the type of the data, regardless of whether it is a pointer or not
     * @param len how many T's the MessageBuf holds
     * @return uint32_t the number of bytes
     */
    template <typename T>
    static uint32_t get_size(std::uint16_t len)
    {
        return sizeof(BufAttrs) + sizeof(T) * len;
    }
    /**
     * @brief Get the number of bytes that a MessageBuf
     * of max size `buf_size` will take
     *
     * @param buf_size the size of the buffer
     * @return uint32_t the number of bytes
     */
    static uint32_t get_size(uint32_t buf_size)
    {
        return sizeof(BufAttrs) + buf_size;
    }

    /**
     * @brief Get the val object
     * @exception Will throw EmptyBufException if the buffer is empty
     * @exception Will throw BadBufCastException if `get_val` is called
     * with the wrong type
     *
     * @tparam T the type of the data, regardless of whether it is a pointer or not
     * @return T the stored value
     */
    template <typename T>
    T get_val()
    {
        // check if it is empty
        if (is_empty())
        {
            throw EmptyBufException();
        }

        // check if it is the right type
        if (typeid(T).hash_code() != get_type_hash())
        {
            throw BadBufCastException();
        }
        // if it is, then return the data
        else
        {
            return reinterpret_cast<T *>(data_holder)[0];
        }
    }

    /**
     * @brief Get the ptr to the val object
     * @exception Will throw EmptyBufException if the buffer is empty
     * @exception Will throw BadBufCastException if `get_val` is called
     * with the wrong type
     *
     * @tparam T the type of the data, regardless of whether it is a pointer or not
     * @return T* a pointer to the data
     */
    template <typename T>
    T *get_ptr_val()
    {
        // check if it is empty
        if (is_empty())
        {
            throw EmptyBufException();
        }

        // check if it is the right type
        if (typeid(T *).hash_code() != get_type_hash())
        {
            throw BadBufCastException();
        }
        else
        {
            return reinterpret_cast<T *>(data_holder);
        }
    }

    /**
     * @brief Store a literal
     *
     * @tparam T the type of the data, regardless of whether it is a pointer or not
     * @param val the value to store
     */
    template <typename T>
    void set_val(T val)
    {
        // clear any data if there is any
        reset();

        // initialize our attributes
        attrs.hold_data<T>(1);

        // then set our data
        data_holder = new T[1]{val};
    }

    /**
     * @brief Set the stored data to an array
     * @details val must be a pointer to an array
     * of T's of minimum length _Length
     *
     * @tparam T the type of the data, regardless of whether it is a pointer or not
     * @param val a pointer to the values to store
     * @param len how many items to store
     */
    template <typename T>
    void set_val(const T *val, std::uint16_t len)
    {
        // clean up
        reset();

        // prepare attributes, this time it IS a pointer
        attrs.hold_data<T>(len, true);

        // initialize our data holder
        data_holder = new T[len];
        memcpy(data_holder, val, sizeof(T) * len);
    }

    /**
     * @brief Convert the buffer to bytes
     *
     * @return char* a pointer to a newly allocated byte buffer that needs to be freed / deleted.
     */
    char *to_bytes() const;

    /**
     * @brief construct this message from bytes
     *
     * @param bytes the bytes
     * @param delete_bytes delete the bytes after using them
     */
    void from_bytes(char *bytes, bool delete_bytes = true);

    /**
     * @brief Copies the other MessageBuf onto this one, including
     * copying the data stored in it
     *
     * @param other the MessageBuf to copy onto this one
     * @return MessageBuf& *this
     */
    MessageBuf &copy(MessageBuf &other);

private:
    /**
     * @brief copies the other MessageBuf, including copying
     * the data stored in it.
     * @details this is a private constructor so that programs
     * will not continually allocate new data. If you want to copy
     * a MessageBuf, call `copy` \see copy
     *
     * @param other the MessageBuf to copy
     */
    MessageBuf(MessageBuf &other);

    /**
     * @brief copies the other MessageBuf onto this one, including
     * copying the data stored in it
     * @details this is a private operator so that programs
     * will not continually allocate new data. If you want to copy
     * a MessageBuf, call `copy` \see copy
     *
     * @param other the MessageBuf to copy
     */
    MessageBuf &operator=(MessageBuf &other);

    /**
     * @brief Struct that MessageBuf uses to store
     * message attributes
     *
     */
    struct BufAttrs
    {
        /**
         * @brief Construct a new BufAttrs object
         *
         */
        BufAttrs();
        /**
         * @brief Construct a new BufAttrs object
         *
         * @param buf_size how large the buffer should be
         */
        BufAttrs(uint32_t buf_size);

        /**
         * @brief configures this attributes object so that
         * it reflects holding data of type T
         *
         * @tparam T the type of the data, regardless of whether it is a pointer or not
         * @param len the length of the message
         * @param is_ptr whether or not this stores a ptr
         */
        template <typename T>
        void hold_data(std::uint16_t len, bool is_ptr = false)
        {
            empty = false;

            if (is_ptr)
            {
                tp_hash = typeid(T *).hash_code();
            }
            else
            {
                tp_hash = typeid(T).hash_code();
            }

            data_holder_len = len;
            data_holder_size = sizeof(T) * data_holder_len;
        }

        /**
         * @brief Resets the attributes
         *
         */
        void reset();

        uint32_t data_holder_size; ///< how large the data is
        uint32_t buf_size;         ///< how large the buffer should be when converting to bytes
        uint16_t data_holder_len;  ///< how many items data contains
        uint64_t tp_hash;          ///< info about type
        bool empty;                ///< whether or not the buffer is empty
    };

    // attributes
    BufAttrs attrs;    ///< the MessageBuf's attributes
    void *data_holder; ///< a pointer to the data; could als be nullptr
};

#endif
/**@}*/