#include "shmcommunicator.hpp"
#include "communication_exception.hpp"
#include "message.hpp"
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
    shm_id = shmget(id, Message::get_num_bytes(max_msg_size), S_IRWXU | S_IRWXO | S_IRWXG | IPC_CREAT);
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

void SHMCommunicator::send_msg(string message)
{
    // get shared memory
    void *buf = shmat(shm_id, NULL, 0);

    // get, write, and delete bytes
    Message m(message);
    char *bytes = m.to_bytes();
    memcpy(buf, reinterpret_cast<const void *>(bytes), Message::get_num_bytes(max_msg_size));
    delete[] bytes;

    // detach from shared memory
    int ret = shmdt(buf);
    check_error(ret, "detaching after send");
}

string SHMCommunicator::receive_msg()
{
    // get shared memory
    void *buf = shmat(shm_id, NULL, 0);

    // create message from it, don't delete the bytes
    Message m;
    m.from_bytes(reinterpret_cast<char *>(buf), false);

    // detach from shared memory
    int ret = shmdt(buf);
    check_error(ret, "detaching after receive");

    return m.get_msg();
}
