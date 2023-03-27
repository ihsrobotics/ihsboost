import ihs_bindings

# create the communicator to use
communicator = ihs_bindings.PosixQCommunicator("/our_queue")

# receiving values in ihsboost python just takes the communicator
ret = ihs_bindings.receive_ints(communicator)
print(ret)

# specify the type to receive by using the function instead of extracting
# it manually like in C++
ret = ihs_bindings.receive_doubles(communicator)
print(ret)

# get a python-style string
ret = ihs_bindings.receive_string(communicator)
print(ret)

# you can receive lists of 1 value
ret = ihs_bindings.receive_ints(communicator)
print(ret)

# and strings of length 1
ret = ihs_bindings.receive_string(communicator)
print(ret)

# same (receives list of 1 bool)
ret = ihs_bindings.receive_bools(communicator)
print(ret)

# automatic typecasting will receive false and true
# because 0 gets typecasted to false and 33 gets typecasted to true
ret = ihs_bindings.receive_bools(communicator)
print(ret)

# this will block since will wait for message the sender will never send
# ret = ihs_bindings.receive_doubles(communicator)
# print(ret)

# receive individual values as individual values
ret = ihs_bindings.receive_int(communicator)
print(ret)
ret = ihs_bindings.receive_bool(communicator)
print(ret)
