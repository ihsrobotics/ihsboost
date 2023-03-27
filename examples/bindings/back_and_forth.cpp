#include <iostream>
#include <ihsboost/all.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

int main()
{
    // we can use the same queue for back and forth communication
    PosixQCommunicator communicator("/my_queue");

    cout << "seinding 33 to python!!" << endl;
    communicator.send_msg(communicator.create_msg<int>(33));

    // sleep before receiving that way we don't receive our own message
    this_thread::sleep_for(milliseconds(500));
    int ret = communicator.receive_msg().get_val<int>();
    cout << "received " << ret << " from python!!" << endl;
    return 0;
}