#include "btcommunicator.hpp"
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::endl;

// ========== BluetoothServer section ==========
BluetoothServer::BluetoothServer(uint32_t max_msg_size)
    : Communicator(max_msg_size),
      bt_socket(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) {
    open();
}
BluetoothServer::BluetoothServer()
    : Communicator(),
      bt_socket(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) {
    open();
}
BluetoothServer::~BluetoothServer() { close(); }
void BluetoothServer::open() {
    // make sure bluetooth socket was created
    check_error(bt_socket, "creating Bluetooth socket");

    // initialize socket properties
    struct sockaddr_rc sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.rc_family = AF_BLUETOOTH;
    sock_addr.rc_channel =
        static_cast<uint8_t>(1); // default rfcomm channel is 1
    sock_addr.rc_bdaddr = {
        0, 0, 0, 0, 0, 0}; // accept any bluetooth address (BDADDR_ANY)
    check_error(bind(bt_socket,
                     reinterpret_cast<struct sockaddr *>(&sock_addr),
                     sizeof(sock_addr)),
                "binding bluetooth socket");

    // listen for 1 connection
    cout << "waiting for bluetooth connection" << endl;
    check_error(listen(bt_socket, 1), "listening for bluetooth connections");

    // accept that connection
    struct sockaddr_rc remote_address;
    socklen_t size = sizeof(remote_address);
    client_fd = accept(
        bt_socket, reinterpret_cast<struct sockaddr *>(&remote_address), &size);
    check_error(client_fd, "accepting bluetooth connection");

    // print connection details
    char buf[30];
    ba2str(&remote_address.rc_bdaddr, buf);
    cout << "accepted connection from " << buf << endl;
}
void BluetoothServer::close() {
    ::close(client_fd);
    ::close(bt_socket);
}
void BluetoothServer::send_bytes(char *bytes) {
    // send bytes
    ssize_t ret = send(client_fd,
                       reinterpret_cast<void *>(bytes),
                       MessageBuf::get_size(max_msg_size),
                       0);
    delete[] bytes; // cleanup

    // check for errors
    check_error(ret, "sending bytes");
}
MessageBuf BluetoothServer::receive_msg() {
    // create the MessageBuf
    MessageBuf m(max_msg_size);

    // receive bytes
    char *buf = new char[MessageBuf::get_size(max_msg_size)];
    ssize_t ret = read(client_fd,
                       reinterpret_cast<void *>(buf),
                       MessageBuf::get_size(max_msg_size));

    // check for errors
    check_error(ret, "receiving bytes");

    // return results
    m.from_bytes(buf);
    return m;
}

// ========== BluetoothClient section ==========
BluetoothClient::BluetoothClient(std::string target_addr, uint32_t max_msg_size)
    : Communicator(max_msg_size),
      target_addr(target_addr),
      bt_socket(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) {
    open();
}
BluetoothClient::BluetoothClient(std::string target_addr)
    : Communicator(),
      target_addr(target_addr),
      bt_socket(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) {
    open();
}
BluetoothClient::~BluetoothClient() { close(); }
void BluetoothClient::open() {
    check_error(bt_socket, "creating Bluetooth socket");

    // parse bt addr
    bdaddr_t bt_addr;
    check_error(str2ba(target_addr.c_str(), &bt_addr),
                "parsing Bluetooth address");

    // initialize socket addr
    struct sockaddr_rc sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.rc_family = AF_BLUETOOTH;
    sock_addr.rc_bdaddr = bt_addr;
    sock_addr.rc_channel = (uint8_t)1; // RFCOMM channel number, defaults to 1
    check_error(connect(bt_socket,
                        reinterpret_cast<struct sockaddr *>(&sock_addr),
                        sizeof(sock_addr)),
                "connecting Bluetooth socket");
}
void BluetoothClient::close() { ::close(bt_socket); }
void BluetoothClient::send_bytes(char *bytes) {
    // send bytes
    ssize_t ret = send(bt_socket,
                       reinterpret_cast<void *>(bytes),
                       MessageBuf::get_size(max_msg_size),
                       0);
    delete[] bytes; // cleanup

    // check for errors
    check_error(ret, "sending bytes");
}
MessageBuf BluetoothClient::receive_msg() {
    // create the MessageBuf
    MessageBuf m(max_msg_size);

    // receive bytes
    char *buf = new char[MessageBuf::get_size(max_msg_size)];
    ssize_t ret = read(bt_socket,
                       reinterpret_cast<void *>(buf),
                       MessageBuf::get_size(max_msg_size));

    // check for errors
    check_error(ret, "receiving bytes");

    // return results
    m.from_bytes(buf);
    return m;
}