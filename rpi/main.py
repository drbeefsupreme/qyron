#!/usr/bin/env python3

#RPC library
from simple_rpc import Interface
import time


VERBOSE = True
SERIAL_PORT = '/dev/ttyACM0'
READ_TIMEOUT = 5

if __name__ == '__main__':
    options = {
        'serial_port': SERIAL_PORT,
        'read_timeout': READ_TIMEOUT,
        'verbose': VERBOSE
    }

    interface = Interface(SERIAL_PORT)

    if interface is None:
        raise Exception('[-] connect_to_serial_port failed')


    print("connection successful")

    time.sleep(3)

    interface.scrollingLayer1_start(b'test', -1)
    time.sleep(1)
    interface.scrollingLayer2_start(b'bweh skrrters', -1)
    time.sleep(1)
    interface.scrollingLayer1_speed(10)
