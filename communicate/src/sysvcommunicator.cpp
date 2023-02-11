#include "sysvcommunicator.hpp"
#include "communication_exception.hpp"
#include <iostream>
#include <fcntl.h>
using namespace std;

SysVCommunicator::SysVCommunicator(const char *path, int identifier, uint32_t max_msg_size) : SysVCommunicator(ftok(path, identifier), max_msg_size){};
SysVCommunicator::SysVCommunicator(int key, uint32_t max_msg_size) : Communicator(max_msg_size), k(key)
{
    open();
}

SysVCommunicator::SysVCommunicator(const char *path, int identifier) : SysVCommunicator(ftok(path, identifier)){};
SysVCommunicator::SysVCommunicator(int key) : Communicator(), k(key)
{
    open();
}

SysVCommunicator::~SysVCommunicator()
{
    close();
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

    int ret = msgctl(msg_q_id, IPC_RMID, NULL);
    try
    {
        check_error(ret, "closing");
    }
    catch (CommunicationException &c)
    {
        if (c.get_error_code() == EINVAL)
        {
            // this happens if the sysv communicator was already closed
            cout << "The SysVCommunicator has already been closed by a different process." << endl;
        }
        else
        {
            throw c;
        }
    }
}

void SysVCommunicator::send_msg(MessageBuf message)
{
    // create message
    char *bytes = message.to_bytes();

    // send the msg
    // the 0 just means don't use any fancy flags
    int ret = msgsnd(msg_q_id, reinterpret_cast<const void *>(bytes), MessageBuf::get_size(max_msg_size), 0);
    check_error(ret, "sending message");

    delete[] bytes;
}

MessageBuf SysVCommunicator::receive_msg()
{
    // create bytes, message
    char *bytes = new char[MessageBuf::get_size(max_msg_size)];
    MessageBuf m(max_msg_size);

    // receive message
    int ret = msgrcv(msg_q_id, bytes, MessageBuf::get_size(max_msg_size), 0, 0);
    check_error(ret, "receiving message");

    // convert byte buffer to message
    m.from_bytes(bytes);
    return m;
}