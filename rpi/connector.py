"""
Representation of the serial connection and reading/writing to it
"""
from helperFunctions import print_verbose
#import HelperFunctions

import serial
import time

class Connector:
    def __init__(self, serial_port='/dev/ttyACM0', baud_rate=9600, read_timeout=5, verbose=True):
        self.conn = None
        self.serial_port = serial_port
        self.read_timeout = read_timeout
        self.baud_rate = baud_rate
        self.verbose = verbose

        self.connect_to_serial_port()

    def get_connection(self) -> serial:
        return self.conn

    def connect_to_serial_port(self):  #-> Boolean?
        # connects to a serial port

        print_verbose('Connecting to port [' + self.serial_port + ']...')

        time.sleep(3)

        try:
            self.conn = serial.Serial(self.serial_port, self.baud_rate)
            self.conn.timeout = self.read_timeout
            print_verbose('Connected')
            return True;
        except Exception as e:
            print('\n\t[-]', e)
            return False

    def get_serial_connection(self):
        # gets the serial connection

        return self.conn

    def close_connection(self):
        # close connection to return port

        print_verbose('Closing connection...', end='')

        try:
            self.conn.close()
            print_verbose('Closed')
            return True;
        except Exception as e:
            print('\n\t[-]', e)
            return False;
