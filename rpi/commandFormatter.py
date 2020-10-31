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

        command = b"".join((tpm2Header,
                  tpm2Command,
                  tpm2BeginText,
                  text.encode(),  #encodes string as raw bytes
                  tpm2Footer))

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
        command = b'\xc9\xc0\x00\x01\x02\x36'
        return command
