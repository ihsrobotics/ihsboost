#include "shmcommunicator.hpp"
#include "communication_exception.hpp"
#include "message.hpp"
#include <sys/shm.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

SHMCommunicator::SHMCommunicator(const char *path, int identifier) : SHMCommunicator(ftok(path, identifier)){};

SHMCommunicator::SHMCommunicator(int id)
{
    shm_id = shmget(id, sizeof(Message), S_IRWXU | S_IRWXO | S_IRWXG | IPC_CREAT);
}

void SHMCommunicator::send_msg(string message)
{
    void *buf = shmat(shm_id, NULL, 0);
    Message m(message);
    memcpy(buf, reinterpret_cast<const void *>(&m), sizeof(Message));
    int ret = shmdt(buf);
    check_error(ret, "detaching after send");
}

string SHMCommunicator::receive_msg()
{
    void *buf = shmat(shm_id, NULL, 0);
    Message m;
    memcpy(reinterpret_cast<void *>(&m), buf, sizeof(Message));
    int ret = shmdt(buf);
    check_error(ret, "detaching after receive");
    return m.get_msg();
}

SHMCommunicator::~SHMCommunicator()
{
    close();
}

void SHMCommunicator::close()
{
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