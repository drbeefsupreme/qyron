#!/usr/bin/env python3

#RPC library
from simple_rpc import Interface
#web app library
from flask import Flask
import time


VERBOSE = True
SERIAL_PORT = '/dev/ttyACM0'
READ_TIMEOUT = 5

app = Flask(__name__)

@app.route("/")
def index():
    return "she a go"

@app.route("/layer1")
def layer1text():
    interface.scrollingLayer1_start(b'Layer 1 test', -1)

@app.route("/layer2")
def layer2text():
    interface.scrollingLayer2_start(b'layer 2 test', -1)

if __name__ == '__main__':
    options = {
        'serial_port': SERIAL_PORT,
        'read_timeout': READ_TIMEOUT,
        'verbose': VERBOSE
    }

    interface = Interface(SERIAL_PORT)

    if interface is None:
        raise Exception('[-] connect_to_serial_port failed')


    print("connection successful")

    time.sleep(1)

    app.run(host='0.0.0.0', port=80, debug=True)
