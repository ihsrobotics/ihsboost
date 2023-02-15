#include "posixqcommunicator.hpp"
#include "communication_exception.hpp"
#include <memory.h>
#include <iostream>

using namespace std;

PosixQCommunicator::PosixQCommunicator(const char *name, size_t max_msgs) : Communicator(), _name(new char[strlen(name)]), max_msgs(max_msgs)
{
    memcpy(reinterpret_cast<void *>(_name), reinterpret_cast<const void *>(name), strlen(name));
    open();
}

PosixQCommunicator::PosixQCommunicator(const char *name, size_t max_msgs, uint32_t max_msg_size) : Communicator(max_msg_size), _name(new char[strlen(name)]), max_msgs(max_msgs)
{
    memcpy(reinterpret_cast<void *>(_name), reinterpret_cast<const void *>(name), strlen(name));
    open();
}

PosixQCommunicator::~PosixQCommunicator()
{
    close();
    delete[] _name;
}

void PosixQCommunicator::open()
{
    // initialize the queue attributes
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = static_cast<int>(max_msgs);
    attr.mq_msgsize = MessageBuf::get_size(max_msg_size);

    msg_q_id = mq_open(_name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
    cout << "msg_q_id is " << msg_q_id << endl;
    check_error(msg_q_id, "opening");
}

void PosixQCommunicator::send_msg(MessageBuf message)
{
    char *bytes = message.to_bytes();
    int ret = mq_send(msg_q_id, reinterpret_cast<const char *>(bytes), MessageBuf::get_size(max_msg_size), 0);
    delete[] bytes;
    check_error(ret, "sending message");
}

MessageBuf PosixQCommunicator::receive_msg()
{
    // create bytes and message
    char *bytes = new char[MessageBuf::get_size(max_msg_size)];
    MessageBuf m(max_msg_size);

    // read into byte buffer
    int ret = mq_receive(msg_q_id, reinterpret_cast<char *>(bytes), MessageBuf::get_size(max_msg_size), 0);
    check_error(ret, "receiving message");

    // create message from bytes
    m.from_bytes(bytes);
    return m;
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
