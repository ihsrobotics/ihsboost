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

#define DEFAULT_MAX_MSG_SIZE 100

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
     * @brief Construct a new MessageBuf object from another MessageBuf
     *
     * @param other the MessageBuf to copy
     */
    MessageBuf(MessageBuf &&other);

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
     * @brief Get the type_info object describing data held
     * @exception Will throw EmptyBufException if the buffer is empty
     *
     * @return const std::type_info&
     */
    const std::uint64_t get_type_hash() const;

    /**
     * @brief Return whether or not the message buf is empty
     *
     * @return true
     * @return false
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
     * @return uint16_t
     */
    uint16_t get_length() const;

    /**
     * @brief Get the number of bytes that a MessageBuf
     * of type T will take that holds `len` number of T's
     * will take
     *
     * @tparam T
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
     * @return uint32_t
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
     * @tparam T the type of the value
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
     *
     * @tparam T
     * @tparam _Len Note that as long as this is provided
     * it should return the correct pointer data, even if
     * `_Len` was incorrect
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
     * @tparam T the type of the object
     * @param val the value to store
     */
    template <typename T>
    void set_val(T val)
    {
        // clear any data if there is any
        reset();

        // initialize our attributes
        attrs.hold_data<T>(1, false);

        // then set our data
        data_holder = new T[1]{val};
    }

    /**
     * @brief Set the stored data to an array
     * @details val must be a pointer to an array
     * of T's of minimum length _Length
     *
     * @tparam T
     * @param val a pointer to the values to store
     * @param len how many items to store
     */
    template <typename T>
    void set_val(const T *val, std::uint16_t len)
    {
        // clean up
        reset();

        // prepare attributes, this time it IS a pointer
        attrs.hold_data<T>(len, false);

        // initialize our data holder
        data_holder = new T[len];
        memcpy(data_holder, val, sizeof(T) * len);
    }

    /**
     * @brief Convert the buffer to bytes
     *
     * @return char*
     */
    char *to_bytes() const;

    /**
     * @brief construct this message from bytes
     *
     * @param bytes the bytes
     * @param delete_bytes delete the bytes after using them
     */
    void from_bytes(char *bytes, bool delete_bytes = true);

private:
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
         * @tparam T
         * @tparam len
         * @param _was_from_bytes whether or not the data was from bytes
         */
        template <typename T>
        void hold_data(std::uint16_t len, bool _was_from_bytes)
        {
            empty = false;
            was_from_bytes = _was_from_bytes;

            if (len > 1) // holding more than 1 val -> its a pointer
            {
                tp_hash = typeid(T *).hash_code();
            }
            else // just holding 1 val -> its a value
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

        uint64_t tp_hash;          ///< info about type
        uint32_t data_holder_size; ///< how large the data is
        uint16_t data_holder_len;  ///< how many items data contains
        uint32_t buf_size;         ///< how large the buffer should be when converting to bytes
        bool empty;                ///< whether or not the buffer is empty
        bool was_from_bytes;       ///< whether or not the buffer was constructed from bytes
    };

    // attributes
    BufAttrs attrs;    ///< the MessageBuf's attributes
    void *data_holder; ///< a pointer to the data; could als be nullptr
};

#endif
/**@}*/