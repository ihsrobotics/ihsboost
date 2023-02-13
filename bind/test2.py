import ihs_communicate

communicator = ihs_communicate.SocketClient("127.0.0.1", 8080, 100)
ret = ihs_communicate.receive_ints(communicator)
print(ret)

ret = ihs_communicate.receive_doubles(communicator)
print(ret)

ret = ihs_communicate.receive_string(communicator)
print(ret)
