/**
 * @file bluetooth_client.cpp
 * @author Eliot Hall
 * @brief File that demonstrates connecting to a bluetooth server
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
    string bt_address =
        "00:11:22:33:44:55"; // replace this with a real bluetooth address
    BluetoothClient client(
        bt_address); // connect to the given bluetooth address

    // receive a message from the server
    int reply = client.receive_msg().get_val<int>();
    cout << "got " << reply << " from our bluetooth connection!" << endl;

    // send a message back
    cout << "sending twice what we got!" << endl;
    client.send_msg(client.create_msg<int>(reply * 2));

    return 0;
}