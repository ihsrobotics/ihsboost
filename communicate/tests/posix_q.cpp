#include <iostream>
#include <mqueue.h>
#include "message.hpp"

using namespace std;

class PosixQCommunicator
{
public:
    PosixQCommunicator(const char *name) : _name(name)
    {
        struct mq_attr attr;
        int must_stop = 0;

        /* initialize the queue attributes */
        attr.mq_flags = 0;
        attr.mq_maxmsg = 10;
        attr.mq_msgsize = 10;
        attr.mq_curmsgs = 0;

        msg_q_id = mq_open("hi", O_CREAT | O_RDONLY, 0644, &attr);
        cout << "msg_q_id is " << msg_q_id << endl;
        if (msg_q_id == -1)
        {
            cerr << "errored!! " << endl;
            cerr << "with error " << errno << endl;
            throw errno;
        }
        EACCES;
    }
    ~PosixQCommunicator()
    {
        int a = mq_close(msg_q_id);
        cout << "closed with value " << a << endl;
        a = mq_unlink("hi");
        cout << "killed with value " << a << endl;
    }

private:
    mqd_t msg_q_id;
    const char *_name;
};

int main()
{
    PosixQCommunicator com{"cool"};
    return 0;
}