import ihs_communicate
import time

# communicator = ihs_communicate.SocketServer(8080, 100)
communicator = ihs_communicate.PosixQCommunicator("/our_queue")

ihs_communicate.send_ints(communicator, [4, 6])
time.sleep(1)
ihs_communicate.send_doubles(communicator, [3.14, 7.22])
time.sleep(1)
ihs_communicate.send_string(communicator, "hi there cutie")
time.sleep(1)

# send individual values
ihs_communicate.send_ints(communicator, [4])
ihs_communicate.send_string(communicator, "h")
ihs_communicate.send_bools(communicator, [True])

# cpp automatic typecasting: <=0 - false, >0 - true
ihs_communicate.send_bools(communicator, [0, 33])

# but cpp automatic typecasting can't deal w/ incompatible types
# the below line will fail
# ihs_communicate.send_doubles(communicator, ["h", "i"])
