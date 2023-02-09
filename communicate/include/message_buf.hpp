#ifndef MESSAGE_BUF_HPP
#define MESSAGE_BUF_HPP
#include <memory.h>
#include <typeinfo>
#include "communication_exception.hpp"

class MessageBuf
{
public:
    // constructors and deconstructors
    MessageBuf() : attrs(), data_holder(nullptr){};
    MessageBuf(MessageBuf &&other)
    {
        // case where other is this
        if (this == &other)
        {
            return;
        }

        // steal attrs
        attrs = other.attrs;
        data_holder = other.data_holder;

        // set the other one to nullptr so
        // that its cleanup doesn't mess with us
        other.data_holder = nullptr;
    }
    ~MessageBuf() { reset(); }

    // modifiers
    /**
     * @brief Cleans up the MessageBuf, deleting any data if there was any
     * and resetting buffer attributes
     *
     */
    void reset()
    {
        // only delete data if it was allocated
        if (!attrs.empty)
        {
            // use the correct delete
            // depends on whether it was constructed from bytes or just a new
            if (attrs.was_from_bytes)
            {
                delete[] reinterpret_cast<char *>(data_holder);
            }
            else
            {
                delete reinterpret_cast<DataHolder<void *> *>(data_holder);
            }
            data_holder = nullptr;
        }

        // clean the other things
        attrs.reset();
    }

    // getters
    /**
     * @brief Get the type_info object describing data held
     * @exception Will throw EmptyBufException if the buffer is empty
     *
     * @return const std::type_info&
     */
    const std::type_info &get_type() const
    {
        // check if empty
        if (is_empty())
        {
            throw EmptyBufException();
        }

        return *reinterpret_cast<const std::type_info *>(attrs.tp_info);
    }

    /**
     * @brief Return whether or not the message buf is empty
     *
     * @return true
     * @return false
     */
    bool is_empty() const
    {
        return attrs.empty;
    }

    uint32_t get_size()
    {
        return sizeof(BufAttrs) + attrs.data_holder_size;
    }

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
    char *to_bytes()
    {
        char *ret = new char[get_size()];

        // copy data attributes
        memcpy(reinterpret_cast<void *>(ret), reinterpret_cast<const void *>(&attrs), sizeof(BufAttrs));

        // copy data if any
        if (!is_empty())
        {
            memcpy(reinterpret_cast<void *>(ret + sizeof(BufAttrs)), reinterpret_cast<const void *>(data_holder), attrs.data_holder_size);
        }

        // return buffer
        return ret;
    }

    /**
     * @brief construct this message from bytes
     *
     * @param bytes the bytes
     * @param delete_bytes delete the bytes after using them
     */
    void from_bytes(char *bytes, bool delete_bytes = true)
    {
        // clean anything we have
        reset();

        // get attrs
        memcpy(reinterpret_cast<void *>(&attrs), reinterpret_cast<const void *>(bytes), sizeof(BufAttrs));

        // get data if any
        if (!is_empty())
        {
            char *temp = new char[attrs.data_holder_size];
            memcpy(reinterpret_cast<void *>(temp), reinterpret_cast<const void *>(bytes + sizeof(BufAttrs)), attrs.data_holder_size);
            data_holder = temp;
            attrs.was_from_bytes = true;
        }

        // cleanup bytes if necessary
        if (delete_bytes)
        {
            delete[] bytes;
            bytes = nullptr;
        }
    }

private:
    // structs that buffer uses
    // message attributes
    struct BufAttrs
    {
        BufAttrs() : empty(true), was_from_bytes(false), tp_info(), data_holder_size(0){};

        template <typename T>
        void hold_data(bool _was_from_bytes)
        {
            empty = false;
            was_from_bytes = _was_from_bytes;
            memcpy(reinterpret_cast<void *>(tp_info), reinterpret_cast<const void *>(&typeid(T)), sizeof(std::type_info));
            data_holder_size = sizeof(DataHolder<T>);
        }

        void reset()
        {
            empty = true;
            data_holder_size = 0;
            was_from_bytes = false;
        }

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