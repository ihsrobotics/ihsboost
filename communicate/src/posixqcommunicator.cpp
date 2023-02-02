#include "posixqcommunicator.hpp"
#include "communication_exception.hpp"
#include <iostream>

using namespace std;

PosixQCommunicator::PosixQCommunicator(const char *name, size_t max_msgs) : _name(name)
{
    // initialize the queue attributes
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = static_cast<int>(max_msgs);
    attr.mq_msgsize = sizeof(Message);

    msg_q_id = mq_open(_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
    cout << "msg_q_id is " << msg_q_id << endl;
    check_error(msg_q_id, "opening");
}

PosixQCommunicator::~PosixQCommunicator()
{
    close();
}

void PosixQCommunicator::send_msg(string message)
{
    Message m(message);
    int ret = mq_send(msg_q_id, reinterpret_cast<const char *>(&m), sizeof(Message), 0);
    check_error(ret, "sending message");
}

string PosixQCommunicator::receive_msg()
{
    Message m("");
    int ret = mq_receive(msg_q_id, reinterpret_cast<char *>(&m), sizeof(Message), 0);
    check_error(ret, "receiving message");
    return m.get_msg();
}

void PosixQCommunicator::close()
{
    cout << "closing and unlinking mqueue" << endl;
    int ret = mq_close(msg_q_id);
    check_error(ret, "closing");

    try
    {
        ret = mq_unlink(_name);
        check_error(ret, "unlinking");
    }
    catch (CommunicationException &c)
    {
        // if the error = because there was no existing file / directory
        if (c.get_error_code() == ENOENT)
        {
            cout << "The mqueue has already been unlinked by a separate program." << endl;
        }
        else
        {
            throw c;
        }
    }
}
