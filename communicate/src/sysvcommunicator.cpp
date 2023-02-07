#include "sysvcommunicator.hpp"
#include "communication_exception.hpp"
#include <iostream>
#include <fcntl.h>
using namespace std;

SysVCommunicator::SysVCommunicator(const char *path, char identifier) : SysVCommunicator(ftok(path, identifier)){};

SysVCommunicator::SysVCommunicator(int key)
{
    k = key;
    cout << "the key is " << k << endl;
    msg_q_id = msgget(k, IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO);
    check_error(msg_q_id, "creating queue");
}

SysVCommunicator::~SysVCommunicator()
{
    close();
}

void SysVCommunicator::send_msg(string message)
{
    // create message
    Message m(message);

    // send the msg
    // the 0 just means don't use any fancy flags
    int ret = msgsnd(msg_q_id, reinterpret_cast<void *>(&m), sizeof(Message), 0);
    check_error(ret, "sending message");
}

string SysVCommunicator::receive_msg()
{
    Message m;
    int ret = msgrcv(msg_q_id, &m, sizeof(Message), 0, 0);
    check_error(ret, "receiving message");
    return m.get_msg();
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