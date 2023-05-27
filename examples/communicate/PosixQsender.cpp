/**
 * @file PosixQsender.cpp
 * @author Eliot Hall
 * @brief Example to demonstrate use of Posix Q's to send values
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <chrono>
#include <ihsboost/all.hpp>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;

class MyDummyClass {
  public:
    MyDummyClass(int a, int b) : value_a(a), value_b(b){};
    int get_value_a() { return value_a; }
    int get_value_b() { return value_b; }
    int get_sum() { return value_a + value_b; }

  private:
    int value_a;
    int value_b;
};

int main() {
    // create a posix q. THe name of the posix q must be similar to this (start
    // with / and be a legitimate file name)
    PosixQCommunicator c("/my_posix_queue");

    // the way to send a message is you have to first create a message and
    // specify its type below, we see that we create a message of type int
    // (because int is in the brackets) and value 33 and then send it
    c.send_msg(c.create_msg<int>(33));
    this_thread::sleep_for(milliseconds(500)); // sleep half a second

    // Or, we can spread out the message creation and sending the message onto
    // multiple lines
    MessageBuf m =
        c.create_msg<double>(3.141592); // create a message of type double
                                        // (since double is in the brackets)
    c.send_msg(m);                      // send the message
    this_thread::sleep_for(milliseconds(500));

    // send a string
    // note that we really send an array of char's that make up the string
    // that's why we do create_msg<char>
    string my_str_msg = "cool message to send"; // create the string
    c.send_msg(c.create_msg<char>(
        my_str_msg.c_str(),
        my_str_msg.size())); // to send a string, send its c_str and its size
    this_thread::sleep_for(milliseconds(500));

    // we can also send other datatypes over communicators.
    // the only restriction is that they CANNOT contain pointers (since we can't
    // guarantee that the receiving side will be on the same process and thus
    // probably won't be able to access those pointers)
    MyDummyClass cool_class(33, 22);
    // create the message (this time we're sending MyDummyClass, so put
    // MyDummyClass in the brackets)
    MessageBuf m_cool = c.create_msg<MyDummyClass>(cool_class);
    // send the message
    c.send_msg(m_cool);
    this_thread::sleep_for(milliseconds(500));

    return 0;
}