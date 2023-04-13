#include "sysvcommunicator.hpp"
#include "communication_exception.hpp"
#include <iostream>
#include <fcntl.h>
using namespace std;

SysVCommunicator::SysVCommunicator(const char *path, int identifier, uint32_t max_msg_size) : SysVCommunicator(ftok(path, identifier), max_msg_size){};
SysVCommunicator::SysVCommunicator(int key, uint32_t max_msg_size) : FileCommunicator(max_msg_size), k(key)
{
    check_exists();
    open();
}

SysVCommunicator::SysVCommunicator(const char *path, int identifier) : SysVCommunicator(ftok(path, identifier)){};
SysVCommunicator::SysVCommunicator(int key) : FileCommunicator(), k(key)
{
    check_exists();
    open();
}

SysVCommunicator::~SysVCommunicator()
{
    close();
}

bool SysVCommunicator::exists()
{
    int ret = msgget(k, S_IRWXU | S_IRWXG | S_IRWXO | IPC_CREAT | IPC_EXCL);
    return ret == -1 && errno == EEXIST;
}

void SysVCommunicator::open()
{
    cout << "the key is " << k << endl;
    msg_q_id = msgget(k, IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
    check_error(msg_q_id, "creating queue");
}

void SysVCommunicator::close()
{
    cout << "closing SysVCommunicator" << endl;

    // only close the file if this is the "owner"
    if (is_owner())
    {
        force_close();
    }
}
void SysVCommunicator::force_close()
{
    int ret = msgctl(msg_q_id, IPC_RMID, NULL);
    check_error(ret, "closing");
}

void SysVCommunicator::send_bytes(char *bytes)
{
    // send the msg
    // the 0 just means don't use any fancy flags
    // (ignore the uint32_t because msgrcv deals with that already)
    int ret = msgsnd(msg_q_id, reinterpret_cast<const void *>(bytes), MessageBuf::get_size(max_msg_size) - sizeof(uint32_t), 0);
    delete[] bytes;
    check_error(ret, "sending message");
}

MessageBuf SysVCommunicator::receive_msg()
{
    // create bytes, message
    char *bytes = new char[MessageBuf::get_size(max_msg_size)];
    MessageBuf m(max_msg_size);

    // receive message (ignore the uint32_t because msgrcv deals with that already)
    ssize_t ret = msgrcv(msg_q_id, bytes, MessageBuf::get_size(max_msg_size) - sizeof(uint32_t), 0, 0);
    check_error(ret, "receiving message");

    // convert byte buffer to message
    m.from_bytes(bytes);
    return m;
}