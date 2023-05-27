#include "communicate.hpp"
#include "test.hpp"
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
using namespace std;
using namespace chrono;

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        cerr << "need to provide whether this is the sender or listener"
             << endl;
        cerr << "for intended results, this should only be called with "
                "'sender' by a client"
             << endl;
        return 1;
    }

    int num_ints = 100;
    int size = sizeof(int) * num_ints;
    if (string(argv[1]) == string("listener")) {
        // set up communicators
        PosixQCommunicator flag_reader("/flag_queue");
        SHMCommunicator reader("/my_shm", 33, MessageBuf::get_size(size));

        // send flag saying we are ready
        flag_reader.send_msg(flag_reader.create_msg<bool>(true));

        for (int i = 0; i < 10; ++i) {
            // wait for ready flag on the posix queue, then read shared memory
            assert_equals(flag_reader.receive_msg().get_val<bool>(),
                          true,
                          "getting ready flag");
            MessageBuf m = reader.receive_msg();

            // check that the sum of every element (excluding the last one) is
            // equal to the final element
            int *back_ptr = m.get_ptr_val<int>() + m.get_length() - 1;
            assert_equals(accumulate(m.get_ptr_val<int>(), back_ptr - 1, 0),
                          *back_ptr,
                          "getting val");
        }
    }
    else if (string(argv[1]) == string("sender")) {
        // set up communicators
        PosixQCommunicator flag_sender("/flag_queue");
        SHMCommunicator sender("/my_shm", 33, MessageBuf::get_size(size));

        // spawn the listener process
        string cmd = string(argv[0]) + " listener &";
        assert_equals(system(cmd.c_str()), 0, "spawning listener process");

        // receive that the spawned process is ready
        assert_equals(flag_sender.receive_msg().get_val<bool>(), true);

        for (int i = 0; i < 10; ++i) {
            vector<int> to_send;
            for (int x = 0; x < num_ints; ++x) {
                to_send.push_back(rand() % 100);
            }

            // put the sum at the end of the vector
            to_send.push_back(accumulate(to_send.begin(), to_send.end(), 0));

            // send the message on shm
            sender.send_msg(sender.create_msg<int>(
                to_send.begin().base(), static_cast<uint16_t>(to_send.size())));

            // send msg on posix queue to let receiver know shm has been updated
            flag_sender.send_msg(flag_sender.create_msg<bool>(true));
            this_thread::sleep_for(milliseconds(500));
        }
    }
    return 0;
}