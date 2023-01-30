#include "sysvcommunicator.hpp"
#include <iostream>
using namespace std;

SysVCommunicator::SysVCommunicator(string path, char identifier) : SysVCommunicator(ftok(path.c_str(), identifier)){};

SysVCommunicator::SysVCommunicator(int key)
{
    k = key;
    cout << "the key is " << k << endl;
    msg_q_id = msgget(k, IPC_CREAT);
    if (msg_q_id == -1)
    {
        cerr << "failed to create queue with error code " << errno << endl;
        cerr << "check errno-base.h for a list of error codes" << endl;
        throw errno;
    }
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
    int result = msgsnd(msg_q_id, reinterpret_cast<void *>(&m), sizeof(Message), 0);
    if (result == -1)
    {
        cerr << "failed to send message with error code " << errno << endl;
        cerr << "check errno-base.h for a list of error codes" << endl;
        throw errno;
    }
}

string SysVCommunicator::receive_msg()
{
    Message m;
    int result = msgrcv(msg_q_id, &m, sizeof(Message), 0, 0);
    if (result == -1)
    {
        cerr << "failed to receive message with error code " << errno << endl;
        cerr << "check errno-base.h for a list of error codes" << endl;
        throw errno;
    }
    return m.get_msg();
}

void SysVCommunicator::close()
{
    msgctl(msg_q_id, IPC_RMID, NULL);
}