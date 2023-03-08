#include "shmcommunicator.hpp"
#include "communication_exception.hpp"
#include <sys/shm.h>
#include <memory.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

SHMCommunicator::SHMCommunicator(const char *path, int identifier, uint32_t max_msg_size) : SHMCommunicator(ftok(path, identifier), max_msg_size){};
SHMCommunicator::SHMCommunicator(int id, uint32_t max_msg_size) : Communicator(max_msg_size), id(id)
{
    open();
}

SHMCommunicator::SHMCommunicator(const char *path, int identifier) : SHMCommunicator(ftok(path, identifier)){};
SHMCommunicator::SHMCommunicator(int id) : Communicator(), id(id)
{
    open();
}

SHMCommunicator::~SHMCommunicator()
{
    close();
}

void SHMCommunicator::open()
{
    shm_id = shmget(id, MessageBuf::get_size(max_msg_size), S_IRWXU | S_IRWXO | S_IRWXG | IPC_CREAT);
    check_error(shm_id, "opening shared memory");
}

void SHMCommunicator::close()
{
    cout << "closing SHMCommunicator" << endl;
    int ret = shmctl(shm_id, IPC_RMID, NULL);
    try
    {
        check_error(ret, "closing");
    }
    catch (CommunicationException &c)
    {
        // allow invalid argument (because shm_id has already been removed)
        // but throw everything else
        if (c.get_error_code() != EINVAL)
        {
            throw c;
        }
    }
}

void SHMCommunicator::send_bytes(char *bytes)
{
    // get shared memory
    void *buf = shmat(shm_id, NULL, 0);

    // write and delete bytes
    memcpy(buf, reinterpret_cast<const void *>(bytes), MessageBuf::get_size(max_msg_size));
    delete[] bytes;

    // detach from shared memory
    int ret = shmdt(buf);
    check_error(ret, "detaching after send");
}

MessageBuf SHMCommunicator::receive_msg()
{
    // get shared memory
    void *buf = shmat(shm_id, NULL, 0);

    // create message from it, don't delete the bytes
    MessageBuf m(max_msg_size);
    m.from_bytes(reinterpret_cast<char *>(buf), false);

    // detach from shared memory
    int ret = shmdt(buf);
    check_error(ret, "detaching after receive");

    return m;
}
