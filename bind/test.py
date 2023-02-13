import ihs_communicate
import time

print(dir(ihs_communicate))

communicator = ihs_communicate.SocketServer(8080, 100)
ihs_communicate.send_ints(communicator, [1, 2, 3, 4])
ihs_communicate.send_doubles(communicator, [3.14, 7.22])
ihs_communicate.send_string(communicator, "hi there cutie")
