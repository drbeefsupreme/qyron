# qyron

This is software for the Raspberry Pi and Teensy 3.6 w/ SmartMatrix LED Shield
v3 used for making SmartMatrix API calls via USB serial.

This shouldn't be used by anyone yet, but some of the patterns may be helpful examples.

## Install instructions

### Raspberry Pi

Install pyserial as root, and run `sudo python3 ./main.py`.

### Teensy 3.6

Install platformio and enter `pio run -t upload` to compile and upload to the Teensy.


## Commands

This library is rapidly evolving so this may not stay up to date. Certainly the
current set of commands is absurd.

The Python library is an abstract interface for sending serial commands to be
hooked up to some kind of front-end. But you can also send commands directly
using a serial monitor.

There are 5 pre-programmed layers.
```
<layerN+text+>
```
prints `text` on layer `N`, where `N=1,2..,5`.
