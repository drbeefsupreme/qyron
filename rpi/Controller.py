"""
Creates a virtual representation of the Arduino and possible API calls to SmartMatrix
"""

from pyduino.HelperFunctions import print_verbose

class Controller:
    def __init__(self, conn=None, verbose=False):
        self.conn = conn
        self.verbose = verbose
        self.state = []

    def set_connection(self, conn):
        # Sets the serial connection for the Controller
        self.conn = conn

    def display_text(self, text)
        # Displays input text

        print_verbose('display_text: ' + text)

        try:
            # command = CommandFormatter.displayText(text)
            # self.conn.write(command.encode())
            print_verbose('command sent')
            self.state = []
            return True
        except Exception as e:
            print('\t[-]', e)
            return False

    def clear_text(self)
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
