#!/usr/bin/env python3

#reads serial messages
import serial #this is actually the pyserial package, installed globally cuz there didnt seem to be another way
import time

#other project files
#from .Connector import Connector
from connector import Connector
from controller import Controller
from helperFunctions import HelperFunctions

#REPL
#from code import InteractiveConsole
#header = "THE QUANTUM METACHYRON"
#footer = "YOU THINK REALITY IS REAL?"

VERBOSE = True
SERIAL_PORT = '/dev/ttyACM0'
READ_TIMEOUT = 1

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

    print("testing...")
    print(conn.name)
    print(conn.is_open)
    print(conn.in_waiting)



"""
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5)
    ser.flush()
    print("test1")
    print(ser.name)
    print(ser.is_open)
    print(ser.in_waiting)

    layer1command = b'\xc9\xc0\x00\x01\x01\x36'
    layer2command = b'\xc9\xc0\x00\x01\x02\x36'
    stringCommand = b'\xc9\xc0\x00\x03' + b' bweh'
    stringCommand2 = b'\xc9\xc0\x00\x03' + b' skrrt'
    #stringText = b'hello'
    #stringPacket = stringCommand + stringText

    time.sleep(1)
    ser.write(stringCommand)
    time.sleep(2)
    ser.write(b'\x36')
    time.sleep(3)
    ser.write(stringCommand)
"""
