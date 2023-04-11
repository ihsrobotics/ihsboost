#include "posixqcommunicator.hpp"
#include "communication_exception.hpp"
#include <memory.h>
#include <iostream>
#include <fstream>

PosixQCommunicator::PosixQCommunicator(std::string name, size_t max_msgs) : FileCommunicator(), _name(name), max_msgs(max_msgs)
{
    check_exists();
    open();
}

PosixQCommunicator::PosixQCommunicator(std::string name, size_t max_msgs, uint32_t max_msg_size) : FileCommunicator(max_msg_size), _name(name), max_msgs(max_msgs)
{
    check_exists();
    open();
}

PosixQCommunicator::~PosixQCommunicator()
{
    close();
}

bool PosixQCommunicator::exists()
{
    std::ifstream file;
    std::string full_name = std::string("/dev/mqueue/") + _name;
    file.open(full_name);
    return static_cast<bool>(file);
}

void PosixQCommunicator::open()
{
    // initialize the queue attributes
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg = static_cast<int>(max_msgs);
    attr.mq_msgsize = MessageBuf::get_size(max_msg_size);

    msg_q_id = mq_open(_name.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, &attr);
    std::cout << "msg_q_id is " << msg_q_id << std::endl;
    check_error(msg_q_id, "opening");
}

void PosixQCommunicator::send_bytes(char *bytes)
{
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
    ssize_t ret = mq_receive(msg_q_id, reinterpret_cast<char *>(bytes), MessageBuf::get_size(max_msg_size), 0);
    check_error(ret, "receiving message");

    // create message from bytes
    m.from_bytes(bytes);
    return m;
}

void PosixQCommunicator::close()
{
    std::cout << "safely closing and unlinking PosixQCommunicator" << std::endl;
    int ret = mq_close(msg_q_id);
    check_error(ret, "closing");

    // only unlink (delete the posix queue) if this is the "owner"
    if (is_owner())
    {
        ret = mq_unlink(_name.c_str());
        check_error(ret, "unlinking");
    }
}
void PosixQCommunicator::force_close()
{
    std::cout << "force closing PosixQCommunicator" << std::endl;
    int ret = mq_close(msg_q_id);
    check_error(ret, "closing");

    ret = mq_unlink(_name.c_str());
    check_error(ret, "unlinking");
}