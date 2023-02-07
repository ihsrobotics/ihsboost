#include "posixqcommunicator.hpp"
#include "communication_exception.hpp"
#include <memory.h>
#include <iostream>

using namespace std;

PosixQCommunicator::PosixQCommunicator(const char *name, size_t max_msgs) : Communicator(), _name(name), max_msgs(max_msgs)
{
    open();
}

PosixQCommunicator::PosixQCommunicator(const char *name, size_t max_msgs, uint32_t max_msg_size) : Communicator(max_msg_size), _name(name), max_msgs(max_msgs)
{
    open();
}

PosixQCommunicator::~PosixQCommunicator()
{
    close();
}

void PosixQCommunicator::open()
{
    // initialize the queue attributes
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = static_cast<int>(max_msgs);
    attr.mq_msgsize = Message::get_num_bytes(max_msg_size);

    msg_q_id = mq_open(_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
    cout << "msg_q_id is " << msg_q_id << endl;
    check_error(msg_q_id, "opening");
}

void PosixQCommunicator::send_msg(string message)
{
    Message m(message, max_msg_size);
    char *bytes = m.to_bytes();
    int ret = mq_send(msg_q_id, reinterpret_cast<const char *>(bytes), Message::get_num_bytes(max_msg_size), 0);
    delete[] bytes;
    check_error(ret, "sending message");
}

string PosixQCommunicator::receive_msg()
{
    // create bytes and message
    char *bytes = new char[max_msg_size];
    Message m;

    // read into byte buffer
    int ret = mq_receive(msg_q_id, reinterpret_cast<char *>(bytes), Message::get_num_bytes(max_msg_size), 0);
    check_error(ret, "receiving message");

    // create message from bytes
    m.from_bytes(bytes);
    return m.get_msg();
}

void PosixQCommunicator::close()
{
    cout << "closing and unlinking PosixQCommunicator" << endl;
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
            cout << "The PosixQCommunicator's mqueue has already been unlinked by a separate program." << endl;
        }
        else
        {
            throw c;
        }
    }
}
