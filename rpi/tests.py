#!/usr/bin/env python3
#test scripts

#reads serial messages
import serial #this is actually the pyserial package, installed globally cuz there didnt seem to be another way
import time

#other project files
#from .Connector import Connector
from connector import Connector
from controller import Controller
from helperFunctions import HelperFunctions
from testStrings import *

VERBOSE = True
SERIAL_PORT = '/dev/ttyACM0'
READ_TIMEOUT = 5

if __name__ == '__main__':
    options = {
        'serial_port': SERIAL_PORT,
        'read_timeout': READ_TIMEOUT,
        'verbose': VERBOSE
    }
    HelperFunctions(options=options)

    connector = Connector(serial_port=SERIAL_PORT, read_timeout=READ_TIMEOUT, verbose=VERBOSE)
    connector.connect_to_serial_port()
    conn = connector.get_serial_connection()



    if conn is None:
        raise Exception('[-] connect_to_serial_port failed')

    controller = Controller(conn=conn, verbose=VERBOSE)


    print("test scripts")
    print("connection name: " + conn.name)
    print("connection open?: " + str(conn.is_open))
    print("connection waiting? " + str(conn.in_waiting))

    time.sleep(3)
    controller.display_text("string length test")
    time.sleep(3)
    controller.display_text("100 characters")
    time.sleep(0.5)
    controller.display_text(hundo,2)
    time.sleep(15)
    controller.display_text("344 characters")
    time.sleep(0.5)
    controller.display_text(threefourfour,2)
