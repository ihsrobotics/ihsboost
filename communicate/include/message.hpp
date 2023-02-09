#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <memory>
#include "message_buf.hpp"

// This is the maximum number of characters in a message.
// note that it it should never exceed 256.
#define MAX_MSG_SIZE 200

class Message
{
public:
    /**
     * @brief Construct a new Message object with no message
     *
     * @param max_size the maximum size of a message that this Message object can hold
     */
    Message(uint32_t max_size = MAX_MSG_SIZE);

    /**
     * @brief Construct a new Message object with the provided message
     *
     * @param msg the message that this Message object will hold
     * @param max_size the maximum size of a message that this Message object can hold
     */
    Message(std::string &msg, uint32_t max_size = MAX_MSG_SIZE);

    /**
     * @brief Construct a new Message object with the provided message
     *
     * @param msg the message that this Message object will hold
     * @param max_size the maximum size of a message that this Message object can hold
     */
    Message(const char *msg, uint32_t max_size = MAX_MSG_SIZE);

    ~Message();

    /**
     * @brief Get the stored message as a string
     *
     * @return std::string - the message that this Message contains
     */
    std::string get_msg();

    /**
     * @brief Get a copy of the bytes that represent this Message object
     *
     * @return char* the bytes
     */
    char *to_bytes();

    /**
     * @brief Constructs this Message object from given bytes
     * @details Resizes this Message object's buffer to correspond to the size
     * of the sent Message in the bytes
     *
     * @param bytes the bytes
     * @param delete_bytes whether or not to delete the bytes after finishing
     */
    void from_bytes(char *bytes, bool delete_bytes = true);

    /**
     * @brief Get how many bytes this message object takes
     *
     * @return uint32_t how many bytes this message object takes
     */
    uint32_t get_num_bytes();

    /**
     * @brief Get how many bytes a message object with max
     * msg size of `max_msg_size` would take
     *
     * @param max_msg_size the max_size of the message
     * @return uint32_t how many bytes it would take
     */
    static uint32_t get_num_bytes(uint32_t max_msg_size);

    struct MessageInfo
    {
        MessageInfo(uint32_t max_size);
        MessageInfo(uint32_t length, uint32_t max_size);

        uint32_t max_size;
        uint32_t length;
    };

private:
    MessageInfo message_info;
    char *buf;
};
#endif