#include "sysvcommunicator.hpp"
#include <iostream>
#include <fcntl.h>
using namespace std;

SysVCommunicator::SysVCommunicator(string path, char identifier) : SysVCommunicator(ftok(path.c_str(), identifier)){};

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
    msgctl(msg_q_id, IPC_RMID, NULL);
}