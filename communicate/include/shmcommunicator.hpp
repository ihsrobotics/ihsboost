#ifndef SHM_COMMUNICATOR_HPP
#define SHM_COMMUNICATOR_HPP

#include "communicator.hpp"

class SHMCommunicator : public Communicator
{
public:
    /**
     * @brief Construct a new SHMCommunicator object. In order to connect to the same msg queue, both `path`
     * and `identifier` must be the same on both communicators
     *
     * @param path a path to real file
     * @param identifier a character to use as the "session id"
     */
    SHMCommunicator(const char *path, int identifier);
    SHMCommunicator(const char *path, int identifier, uint32_t max_msg_size);

    /**
     * @brief Construct a new SHMCommunicator object. In order to connect to the same shared memory,
     * the provided key on both communicators must be the same
     *
     * @param key
     */
    SHMCommunicator(int id);
    SHMCommunicator(int id, uint32_t max_msg_size);

    virtual ~SHMCommunicator();

    /**
     * @brief Send a message
     * @exception Segmentation Fault - happens if the shared memory has already been
     * closed by another process
     *
     * @param message the message to send
     */
    virtual void send_msg(std::string message);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     * @exception Segmentation Fault - happens if the shared memory has already been
     * closed by another process
     *
     * @return std::string - the message that was received
     */
    virtual std::string receive_msg();

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close();

    virtual void open();

private:
    int id;
    int shm_id;
};

#endif