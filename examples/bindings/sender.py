import ihs_bindings
import time

# create the communicator to use
communicator = ihs_bindings.PosixQCommunicator("/our_queue")

# send lists and full strings
ihs_bindings.send_ints(communicator, [4, 6])
time.sleep(1)
ihs_bindings.send_doubles(communicator, [3.14, 7.22])
time.sleep(1)
ihs_bindings.send_string(communicator, "hi there cutie")
time.sleep(1)

# send individual values
ihs_bindings.send_ints(communicator, [4])
ihs_bindings.send_string(communicator, "h")
ihs_bindings.send_bools(communicator, [True])

# cpp automatic typecasting: <=0 - false, >0 - true
ihs_bindings.send_bools(communicator, [0, 33])

# but cpp automatic typecasting can't deal w/ incompatible types
# the below line will fail
# ihs_bindings.send_doubles(communicator, ["h", "i"])
