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
READ_TIMEOUT = 5

##if __name__ == '__main__':
def main():
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

    print("name, open, waiting:")
    print(conn.name)
    print(conn.is_open)
    print(conn.in_waiting)

    time.sleep(3)
    controller.display_text("advancedness")
    time.sleep(3)
    controller.display_text("layer2",2)
