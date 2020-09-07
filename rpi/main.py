#!/usr/bin/env python3

#reads serial messages
import serial

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=5)
    ser.flush()
    print("test1")
    print(ser.name)
    print(ser.is_open)
    print(ser.in_waiting)

    while True:
        if ser.in_waiting > 0:
            print("test")
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
