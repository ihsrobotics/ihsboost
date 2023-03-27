from ihs_bindings import *

communicator = PosixQCommunicator("/my_queue")

# receive a value from c++
val = receive_int(communicator)
print("received", val, "from c++!!!")

# send a value to c++ over the same queue
print("sending 23 to c++!")
send_int(communicator, 23)
