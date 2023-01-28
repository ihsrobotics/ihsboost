#include <iostream>
#include <fstream>
#include <sys/msg.h>
#include <sys/shm.h>
#include <memory.h>

using namespace std;

// create a communicator
// should be able to sendmessage
// should have ability to parse config msgs

#define MAX_MSG_SIZE 200
class Message
{
public:
    Message(){};
    Message(string msg) : length(msg.size())
    {
        memcpy(buf, msg.c_str(), length * sizeof(char));
    }
    string get_msg()
    {
        string ret = "";
        for (size_t i = 0; i < length; ++i)
        {
            ret.push_back(buf[i]);
        }
        return ret;
    }

private:
    size_t length;
    char buf[MAX_MSG_SIZE];
};

class MSGCommunicator
{
public:
    /**
     * @brief Construct a new MSGCommunicator object. In order to connect to the same msg queue, both `path`
     * and `identifier` must be the same on both communicators
     *
     * @param path a path to real file
     * @param identifier a character to use as the "session id"
     */
    MSGCommunicator(string path, char identifier)
    {
        k = ftok(path.c_str(), identifier);
        cout << "the key is " << k << endl;
        msg_q_id = msgget(k, IPC_CREAT);
        if (msg_q_id == -1)
        {
            cerr << "failed to create queue with error code " << errno << endl;
            cerr << "check errno-base.h for a list of error codes" << endl;
            throw errno;
        }
    }
    ~MSGCommunicator()
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

    MSGCommunicator a("../../README.md", 'c');
    MSGCommunicator b("../../README.md", 'c');

    a.send_msg("hi there bay area!");
    a.send_msg("you are listening to 95.7 KOIT");
    cout << b.receive_msg() << endl;
    cout << b.receive_msg() << endl;
    return 0;
}