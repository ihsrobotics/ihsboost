import ihs_communicate
import time

# communicator = ihs_communicate.SocketServer(8080, 100)
communicator = ihs_communicate.PosixQCommunicator("/our_queue")

ihs_communicate.send_ints(communicator, [1, 2, 3, 4])
time.sleep(1)
ihs_communicate.send_doubles(communicator, [3.14, 7.22])
time.sleep(1)
ihs_communicate.send_string(communicator, "hi there cutie")
time.sleep(1)
