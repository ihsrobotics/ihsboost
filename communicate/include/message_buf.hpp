#ifndef MESSAGE_BUF_HPP
#define MESSAGE_BUF_HPP
#include <memory.h>
#include <typeinfo>
#include "communication_exception.hpp"

class MessageBuf
{
public:
    // constructors and deconstructors
    MessageBuf();
    MessageBuf(MessageBuf &&other);
    ~MessageBuf();

    // modifiers
    /**
     * @brief Cleans up the MessageBuf, deleting any data if there was any
     * and resetting buffer attributes
     *
     */
    void reset();

    // getters
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
     * object takes
     *
     * @return uint32_t the number of bytes
     */
    uint32_t get_size() const;

    /**
     * @brief Get the number of bytes that a MessageBuf
     * of type T will take
     *
     * @tparam T
     * @return uint32_t the number of bytes
     */
    template <typename T, std::uint16_t _Len = 1>
    static uint32_t get_size()
    {
        return sizeof(BufAttrs) + sizeof(DataHolder<T, _Len>);
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
            return reinterpret_cast<DataHolder<T, 1> *>(data_holder)->val[0];
        }
    }
    template <typename T, std::uint16_t _Len>
    T *get_val()
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
            return reinterpret_cast<DataHolder<T, _Len> *>(data_holder)->val;
        }
    }

    // setters
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
        attrs.hold_data<T>(false, false);

        // then set our data
        data_holder = new DataHolder<T, 1>(val);
    }

    /**
     * @brief Set the stored data to an array
     * @details val must be a pointer to an array
     * of T's of minimum length _Length
     *
     * @tparam T
     * @tparam _Length
     * @param val
     */
    template <typename T, std::uint16_t _Length>
    void set_val(T *val)
    {
        // clean up
        reset();

        // prepare attributes, this time it IS a pointer
        attrs.hold_data<T, _Length>(true, false);

        // initialize our data holder
        data_holder = new DataHolder<T, _Length>(val);
    }

    // byte stuff
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
    // structs that buffer uses
    // message attributes
    struct BufAttrs
    {
        BufAttrs();

        /**
         * @brief configures this attributes object so that
         * it reflects holding data of type T
         *
         * @tparam T
         * @tparam len
         * @param _was_from_bytes whether or not the data was from bytes
         */
        template <typename T, std::uint16_t _Len = 1>
        void hold_data(bool is_ptr, bool _was_from_bytes)
        {
            empty = false;
            was_from_bytes = _was_from_bytes;

            if (is_ptr)
            {
                tp_hash = typeid(T *).hash_code();
            }
            else
            {
                tp_hash = typeid(T).hash_code();
            }

            data_holder_len = _Len;
            data_holder_size = sizeof(DataHolder<T, _Len>);
        }

        /**
         * @brief Resets the attributes
         *
         */
        void reset();

        bool empty;                // whether or not the buffer is empty
        bool was_from_bytes;       // whether or not the buffer was constructed from bytes
        uint64_t tp_hash;          // info about type
        uint32_t data_holder_size; // how large the data is
        uint16_t data_holder_len;  // how many items data contains
    };

    // data attributes
    template <typename T, std::uint16_t len>
    struct DataHolder
    {
        DataHolder(T &&_val) : val{_val} {};
        DataHolder(T &_val) : val{_val} {};
        DataHolder(T *_val) : val()
        {
            for (uint16_t i = 0; i < len; ++i)
            {
                val[i] = _val[i];
            }
        }

        T val[len];
    };

    // attributes
    BufAttrs attrs;
    void *data_holder;
};

#endif