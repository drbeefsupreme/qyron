#!/usr/bin/env python3

#reads serial messages
import serial #this is actually the pyserial package, installed globally cuz there didnt seem to be another way
import time

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5)
    ser.flush()
    print("test1")
    print(ser.name)
    print(ser.is_open)
    print(ser.in_waiting)

    layer1command = b'\xc9\xc0\x00\x01\x01\x36'
    layer2command = b'\xc9\xc0\x00\x01\x02\x36'

    time.sleep(1)
    ser.write(layer1command)
    time.sleep(5)
    ser.write(layer2command)
