#ifndef SHM_COMMUNICATOR_HPP
#define SHM_COMMUNICATOR_HPP

#include "communicator.hpp"

/**
 * @brief Class for communicating over shared memory
 *
 */
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

    /**
     * @brief Construct a new SHMCommunicator object. In order to connect to the same msg queue, both `path`
     * and `identifier` must be the same on both communicators
     *
     * @param path a path to real file
     * @param identifier a character to use as the "session id"
     * @param max_msg_size The maximum size of your messages
     */
    SHMCommunicator(const char *path, int identifier, uint32_t max_msg_size);

    /**
     * @brief Construct a new SHMCommunicator object. In order to connect to the same shared memory,
     * the provided id on both communicators must be the same
     *
     * @param id the id for this SHMCommunicator.
     */
    SHMCommunicator(int id);

    /**
     * @brief Construct a new SHMCommunicator object. In order to connect to the same shared memory,
     * the provided id on both communicators must be the same
     *
     * @param id the id for this SHMCommunicator.
     * @param max_msg_size The maximum size of your messages
     */
    SHMCommunicator(int id, uint32_t max_msg_size);

    /**
     * @brief Destroy the SHMCommunicator object and the shared memory associated
     * with it
     *
     */
    virtual ~SHMCommunicator();

    /**
     * @brief Opens the communicator
     * @details This should be called automatically in the constructor
     *
     */
    virtual void open();

    /**
     * @brief Close the communicator.
     * @details This should be called automatically in the deconstructor.
     *
     */
    virtual void close();

    /**
     * @brief Send a message
     * @exception Segmentation Fault - happens if the shared memory has already been
     * closed by another process
     *
     * @param message the message to send
     */
    virtual void send_msg(MessageBuf message);

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     * @exception Segmentation Fault - happens if the shared memory has already been
     * closed by another process
     *
     * @return std::string - the message that was received
     */
    virtual MessageBuf receive_msg();

private:
    int id;     ///< the id of the SHMCommunicator
    int shm_id; ///< the id of the attached shared memory
};

#endif