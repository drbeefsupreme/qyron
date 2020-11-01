"""
Formats commands into strings to be written to the serial port
"""

from constants import *

class CommandFormatter:
    @staticmethod
    def display_text_command(text):
        """
        Formats a command for displaying text
        """

        if type(text) is not str:
            return ''

        command = b"".join((b'<print+',
                            text.encode(),
                            b'+>'))
        return command

    @staticmethod
    def test_layer_1():
        # debugging

        """
        command = b"".join((tpm2Header,
                  tpm2Command,
                  tpm2Layer1))
                  #tpm2Footer))
        """

        #command = b'\xc9\xc0\x00\x01\x01\x36'
        #command = "<print2+test layer 1>"

        command = b'<print2+testlayer1+>'

        return command
