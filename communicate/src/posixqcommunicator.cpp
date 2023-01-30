#include "posixqcommunicator.hpp"
#include <iostream>

using namespace std;

PosixQCommunicator::PosixQCommunicator(const char *name, size_t max_msgs) : _name(name)
{
    // initialize the queue attributes
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = static_cast<int>(max_msgs);
    attr.mq_msgsize = sizeof(Message);

    msg_q_id = mq_open(_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, &attr);
    cout << "msg_q_id is " << msg_q_id << endl;
    if (msg_q_id == -1)
    {
        cerr << "errored while opening with error " << errno << endl;
        throw errno;
    }
}

PosixQCommunicator::~PosixQCommunicator()
{
    close();
}

void PosixQCommunicator::send_msg(string message)
{
    Message m(message);
    int ret = mq_send(msg_q_id, reinterpret_cast<const char *>(&m), sizeof(Message), 0);
    if (ret == -1)
    {
        cerr << "errored while sending message with error " << errno << endl;
        throw errno;
    }
}

string PosixQCommunicator::receive_msg()
{
    Message m("");
    int ret = mq_receive(msg_q_id, reinterpret_cast<char *>(&m), sizeof(Message), 0);
    if (ret == -1)
    {
        cerr << "errored while receiving message with error " << errno << endl;
        throw errno;
    }
    return m.get_msg();
}

void PosixQCommunicator::close()
{
    int ret = mq_close(msg_q_id);
    if (ret == -1)
    {

        cerr << "errored while closing with error " << errno << endl;
        throw errno;
    }

    ret = mq_unlink(_name);
    if (ret == -1)
    {
        cerr << "errored while unlinking message queue with error " << errno << endl;
        throw errno;
    }
}
