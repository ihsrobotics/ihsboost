#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <memory>

// This is the maximum number of characters in a message.
// note that it it should never exceed 256.
#define MAX_MSG_SIZE 200

class Message
{
public:
    /**
     * @brief Construct a new Message object with no message
     *
     */
    Message(uint32_t max_size = MAX_MSG_SIZE);

    Message(std::string &msg, uint32_t max_size = MAX_MSG_SIZE);

    /**
     * @brief Construct a new Message object with the provided message
     *
     * @param msg the message that this Message object will hold
     */
    Message(const char *msg, uint32_t max_size = MAX_MSG_SIZE);

    ~Message();

    /**
     * @brief Get the stored message as a string
     *
     * @return std::string - the message that this Message contains
     */
    std::string get_msg();

    char *to_bytes();

    void from_bytes(char *bytes, bool delete_bytes = true);

    uint32_t get_num_bytes();

private:
    uint32_t _max_size;
    uint32_t length;
    char *buf;
};
#endif