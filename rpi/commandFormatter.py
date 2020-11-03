"""
Formats commands into strings to be written to the serial port
"""

from constants import *

class CommandFormatter:
    @staticmethod
    def display_text_command(text, layer):
        """
        Formats a command for displaying text
        """

        if type(text) is not str:
            return ''

        command = b"".join((b'<layer',
                            str(layer).encode(),
                            b'+',
                            text.encode(),
                            b'+>'))
        return command

    @staticmethod
    def clear():
        """
        Clears the matrix
        """

        command = b'<clear++>'

        return command

