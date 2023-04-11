/**
 * @file shmcommunicator.hpp
 * @author Eliot Hall
 * @brief Shared memory based communication
 * @version 0.1
 * @date 2023-02-14
 *
 * @copyright Copyright (c) 2023
 *
 * @addtogroup communication_id
 * @{
 */
#ifndef IHSBOOST_SHM_COMMUNICATOR_HPP
#define IHSBOOST_SHM_COMMUNICATOR_HPP

#include "communicator.hpp"

/**
 * @brief Class for communicating over shared memory
 *
 */
class SHMCommunicator : public FileCommunicator
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
     * @brief Close the communicator and remove the associated shm file
     * even if this isn't the owner
     *
     */
    virtual void force_close();

    /**
     * @brief Wait to receive a message.
     * Blocks until message was received
     * @exception Segmentation Fault - happens if the shared memory has already been
     * closed by another process
     *
     * @return MessageBuf - the message that was received
     */
    virtual MessageBuf receive_msg();

private:
    /**
     * @brief Send the bytes of the MessageBuf over the communicator
     * @warning bytes will be deleted
     * @exception Segmentation Fault - happens if the shared memory has already been
     * closed by another process
     *
     * @param bytes the bytes, created from MessageBuf.to_bytes
     */
    virtual void send_bytes(char *bytes);

    /**
     * @brief Check if the shm file already exists
     *
     * @return true
     * @return false
     */
    virtual bool exists();

    int id;     ///< the id of the SHMCommunicator
    int shm_id; ///< the id of the attached shared memory
};

#endif
/**@}*/