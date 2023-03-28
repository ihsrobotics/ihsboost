from ihs_bindings import *
from time import sleep
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("communicator", type=str)

if __name__ == "__main__":
    args = parser.parse_args()

    # initialize communicator
    if args.communicator == "PosixQCommunicator":
        com = PosixQCommunicator("/back_and_forth_queue")
    elif args.communicator == "SocketCommunicator":
        com = SocketClient("127.0.0.1", 2222)
    elif args.communicator == "SysVCommunicator":
        com = SysVCommunicator(22)
    else:
        print("invalid communicator")
        exit(2)

    # send ready flag
    send_int(com, 1)

    # modify a value and send it back
    multiplier = 23
    val = receive_int(com)
    print("received", val)
    send_int(com, val * multiplier)
    print("sent", val * multiplier)
    sleep(0.02)

    # modify a string and send it back
    val = receive_string(com)
    print("received", val)
    val += " world"
    print("sending", val)
    send_string(com, val)

    # sleep a bit before exiting
    sleep(1)
