/**
 * @file bluetooth_sender.cpp
 * @author Eliot Hall
 * @brief File that demonstrates an example bluetooth server
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <ihsboost/all.hpp>
#include <iostream>

using namespace std;

int main() {
    // create a server and await a bluetooth connection
    BluetoothServer server;

    cout << "Received a connection, so moving on" << endl;
    server.send_msg(
        server.create_msg<int>(100)); // send a message over bluetooth

    cout << "waiting for reply" << endl;
    int result = server.receive_msg()
                     .get_val<int>(); // wait for a reply message over bluetooth
    cout << "got " << result << " from bluetooth!" << endl;

    return 0;
}