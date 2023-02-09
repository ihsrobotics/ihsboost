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
    const std::type_info &get_type() const;

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
    template <typename T>
    static uint32_t get_size()
    {
        return sizeof(BufAttrs) + sizeof(DataHolder<T>);
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
        if (typeid(T) != get_type())
        {
            throw BadBufCastException();
        }
        // if it is, then return the data
        else
        {
            return reinterpret_cast<DataHolder<T> *>(data_holder)->val;
        }
    }

    // setters
    /**
     * @brief Set the val object
     *
     * @tparam T the type of the object
     */
    template <typename T>
    void set_val(T val)
    {
        // clear any data if there is any
        reset();

        // initialize our attributes
        attrs.hold_data<T>(false);

        // then set our data
        data_holder = new DataHolder<T>(val);
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
         * @param _was_from_bytes whether or not the data was from bytes
         */
        template <typename T>
        void hold_data(bool _was_from_bytes)
        {
            empty = false;
            was_from_bytes = _was_from_bytes;
            memcpy(reinterpret_cast<void *>(tp_info), reinterpret_cast<const void *>(&typeid(T)), sizeof(std::type_info));
            data_holder_size = sizeof(DataHolder<T>);
        }

        /**
         * @brief Resets the attributes
         *
         */
        void reset();

        bool empty;                           // whether or not the buffer is empty
        bool was_from_bytes;                  // whether or not the buffer was constructed from bytes
        char tp_info[sizeof(std::type_info)]; // info about type
        uint32_t data_holder_size;            // how large the data is
    };

    // data attributes
    template <typename T>
    struct DataHolder
    {
        DataHolder(T &&val) : val(val){};
        DataHolder(T &val) : val(val){};
        T val;
    };

    // attributes
    BufAttrs attrs;
    void *data_holder;
};

#endif