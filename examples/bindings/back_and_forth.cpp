/**
 * @file back_and_forth.cpp
 * @author Eliot Hall
 * @brief example communication between python and
 * c++ where the c++ program calls the python program.
 * This can be run by merely compiling this and running
 * the created executable
 * @version 0.1
 * @date 2023-03-27
 *
 * @copyright Copyright (c) 2023
 *
 */

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

    // launch python program
    system("python3 ./back_and_forth.py &");

    // wait for python to let us know it is ready
    cout << "waiting for python to be ready" << endl;
    int status = communicator.receive_msg().get_val<int>();
    if (status != 1)
    {
        return status;
    }
    cout << "received a 1 from python letting us know it's ready!!" << endl;

    cout << "seinding 33 to python!!" << endl;
    communicator.send_msg(communicator.create_msg<int>(33));

    // sleep before receiving that way we don't receive our own message
    this_thread::sleep_for(milliseconds(500));
    int ret = communicator.receive_msg().get_val<int>();
    cout << "received " << ret << " from python!!" << endl;
    return 0;
}