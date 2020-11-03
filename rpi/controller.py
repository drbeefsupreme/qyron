"""
Creates a virtual representation of the Arduino and possible API calls to SmartMatrix
"""

from helperFunctions import print_verbose
from commandFormatter import CommandFormatter

class Controller:
    def __init__(self, conn=None, verbose=False):
        self.conn = conn
        self.verbose = verbose
        self.state = []

    def set_connection(self, conn):
        # Sets the serial connection for the Controller
        self.conn = conn

    def display_text(self, text, layer=1):
        # Displays input text

        print_verbose('display_text: ' + text + ' layer ' + str(layer))

        try:
            command = CommandFormatter.display_text_command(text, layer)
            self.conn.write(command)
            print_verbose('command sent')
            self.state = []
            return True
        except Exception as e:
            print('\t[-]', e)
            return False

    def clear_text(self):
        #clears the screen of text

        print_verbose('clear_text')

        try:
            # command = CommandFormatter.clearText()
            # self.conn.write(command.encode())
            print_verbose('text cleared')
            self.state = []
            return True;
        except Exception as e:
            print('\t[-]', e)
            return False;

    def test_layer_1(self):
        # testing

        print_verbose('testing layer 1')

        try:
            command = CommandFormatter.test_layer_1()
            print(command)
            self.conn.write(command)
            print_verbose('command sent')
            self.state = []
            return True

        except Exception as e:
            print('test layer fail')
            print('\t[-]', e)
            return False;
