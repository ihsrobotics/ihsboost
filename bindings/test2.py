import ihs_bindings

# communicator = ihs_bindings.SocketClient("127.0.0.1", 8080, 100)
communicator = ihs_bindings.PosixQCommunicator("/our_queue")

ret = ihs_bindings.receive_ints(communicator)
print(ret)

ret = ihs_bindings.receive_doubles(communicator)
print(ret)

ret = ihs_bindings.receive_string(communicator)
print(ret)

ret = ihs_bindings.receive_ints(communicator)
print(ret)
ret = ihs_bindings.receive_string(communicator)
print(ret)
ret = ihs_bindings.receive_bools(communicator)
print(ret)

ret = ihs_bindings.receive_bools(communicator)
print(ret)

# ret = ihs_bindings.receive_doubles(communicator)
# print(ret)
