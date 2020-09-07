# qyron

This is software for the Raspberry Pi and Teensy 3.6 w/ SmartMatrix LED Shield
v3 used for making SmartMatrix API calls via TPM2 connected via USB serial.

## Install instructions

### Raspberry Pi

Install pyserial as root, and run `sudo python3 ./main.py`.

### Teensy 3.6

Install platformio and enter `pio run -t upload` to compile and upload to the Teensy.
