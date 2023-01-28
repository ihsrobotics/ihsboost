#include <iostream>
#include <sys/msg.h>
#include "message.hpp"

using namespace std;

// create a communicator
// should be able to sendmessage
// should have ability to parse config msgs

/**
 * @brief Class to communicate using System V message queues
 *
 */
class SysVCommunicator
{
public:
    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue, both `path`
     * and `identifier` must be the same on both communicators
     *
     * @param path a path to real file
     * @param identifier a character to use as the "session id"
     */
    SysVCommunicator(string path, char identifier) : SysVCommunicator(ftok(path.c_str(), identifier)){};
    /**
     * @brief Construct a new SysVCommunicator object. In order to connect to the same msg queue,
     * the provided key on both communicators must be the same
     *
     * @param key
     */
    SysVCommunicator(int key)
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
    ~SysVCommunicator()
    {
        close();
    }

    void send_msg(string message)
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

    string receive_msg()
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

    void close()
    {
        msgctl(msg_q_id, IPC_RMID, NULL);
    }

private:
    key_t k;
    int msg_q_id;
};
int main()
{
    // Message my_msg("hi there");
    // cout << my_msg.get_msg() << endl;
    // cout << "size of message is " << sizeof(Message) << " and of our particular message is " << sizeof(my_msg) << endl;

    SysVCommunicator a("../../README.md", 'd');
    SysVCommunicator b("../../README.md", 'd');

    a.send_msg("hi there bay area!");
    a.send_msg("you are listening to 95.7 KOIT");
    cout << b.receive_msg() << endl;
    cout << b.receive_msg() << endl;
    return 0;
}