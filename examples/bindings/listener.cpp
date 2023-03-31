/**
 * @file listener.cpp
 * @author Eliot Hall
 * @brief FIle to listen on the c++ side for messages sent by python
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <ihsboost/all.hpp>
#include <iostream>
using namespace std;

int main()
{
    PosixQCommunicator communicator("/our_queue");

    // receive the list of ints
    MessageBuf ret = communicator.receive_msg();
    for (size_t i = 0; i < ret.get_length(); ++i)
    {
        cout << ret.get_ptr_val<int>()[i] << ", ";
    }
    cout << endl;

    // receive the list of doubles
    ret = communicator.receive_msg();
    for (size_t i = 0; i < ret.get_length(); ++i)
    {
        cout << ret.get_ptr_val<double>()[i] << ", ";
    }
    cout << endl;

    // receive the string
    ret = communicator.receive_msg();
    cout << ret.get_ptr_val<char>() << endl;

    // receive the individual int (that was sent as a list)
    ret = communicator.receive_msg();
    cout << ret.get_ptr_val<int>()[0] << endl;

    // receive the individual char
    ret = communicator.receive_msg();
    cout << ret.get_ptr_val<char>()[0] << endl;

    // receive the individual bool
    ret = communicator.receive_msg();
    cout << ret.get_ptr_val<bool>()[0] << endl;

    // receive the typecasted bools
    ret = communicator.receive_msg();
    for (size_t i = 0; i < ret.get_length(); ++i)
    {
        cout << ret.get_ptr_val<bool>()[i] << ", ";
    }
    cout << endl;

    // receive the single vals
    int val = communicator.receive_msg().get_val<int>();
    cout << "got the single integer of " << val << endl;
    bool other_val = communicator.receive_msg().get_val<bool>();
    cout << "got the single bool of " << other_val << endl;
}