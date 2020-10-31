# constants for the TPM2 protocol
# These should match what is on the arduino side

tpm2Header = b'\xc9'
tpm2netHeader = b'\x9c'
tpm2DataFrame = b'\xda'
tpm2Command = b'\xc0'
tpm2Answer = b'\xaa'
tpm2Footer = b'\x36'
tpm2Ack = b'\xac'

# commands
tpm2Layer1 = b'\x01'
tpm2Layer2 = b'\x02'
tpm2BeginText = b'\x03'
tpm2EndText = b'\xaa'
