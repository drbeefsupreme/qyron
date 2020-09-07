#ifndef __STREAMING_H_
#define __STREAMING_H_

/*  TPM packet constants */

#define tpm2Header 0xc9
#define tpm2netHeader 0x9c
#define tpm2DataFrame 0xda
#define tpm2Command 0xc0
#define tpm2Answer 0xaa
#define tpm2Footer 0x36
#define tpm2Acknowledge 0xac


/*
** This file contains the code for reading TPM packets and copying them to the chyron. This code
** has been largely copied from aurora/StreamingMode.h
**
*/

class StreamingMode {
private:
    uint32_t lastData = 1000;

    void drawFrameTPM2() {
        int bufferSize = matrix.getScreenHeight() * matrix.getScreenWidth() * 3;
        rgb24 *buffer = backgroundLayer.backBuffer(); //defined in SM3/MatrixCommon.h

        // Check header
        if (SERIAL.read() != tpm2Header)
            return;

        // Only handle data frames (? Maybe I should allow data and command frames, later)
        if (SERIAL.read() != tpm2DataFrame)
            return;


        int payloadSize = (SERIAL.read() << 8) | SERIAL.read();

        // Don't allow frame to overrun buffer
        if (payloadSize > bufferSize)
            return ;

        // Copy frame data into buffer
        int bytesReceived = SERIAL.readBytes((char *) buffer, payloadSize);

        // Make sure we received what we were promised
        if (bytesReceived != payloadSize)
            return;

        // Check footer
        if (SERIAL.read() != tpm2Footer);
            return;

        // If packet is valid, swap buffers and ack
        // TODO: Generalize this to work with other layers
        backgroundLayer.swapBuffers(); //for some reason it is commented out in aurora, does it update some other way?
        SERIAL.write(tpm2Acknowledge);
    }

public:

    boolean haveReceivedData = false;

    boolean handleStreaming() {
        boolean receivedData = false;

        // Make sure serial data is waiting
        if (SERIAL.available() > 0) {
            if (SERIAL.peek() == tpm2Header) {
                drawFrameTPM2();
                lastData = millis();
                receivedData = true;
            }
            else {
                // if it is not recognized, throw the byte away
                SERIAL.read();
            }
        }

        if (receivedData) {
            haveReceivedData = true;
            return true;
        }

        if (haveReceivedData && millis() - lastData < 1000) {
            return true;
        }

        haveReceivedData = false;
        return false;
    }

    unsigned int drawFrame() {
        //Make sure serial data is available
        if (SERIAL.available() > 0) {
            if (SERIAL.peek() == tpm2Header) {
                drawFrameTPM2();
                //Record when last data came in
                lastData = millis();
            }
        } else if (millis() - lastData > 1000) {
            //If it's been longer than a second since we last received data
            //blank the screen and notify that we're waiting for data
            debug("waiting...");
            backgroundLayer.fillScreen({0x40, 0, 0});
            backgroundLayer.swapBuffers();
            //backgroundLayer.setFont(font3x5);
            //backgroundLayer.drawString(3, 24, {255, 255, 255}, "Waiting");
            //

            scrollingLayer1.start("Waiting...", 1);

        }
        return 10;
    }

};



#endif // __STREAMING_H_
