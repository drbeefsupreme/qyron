#ifndef __STREAMING_H_
#define __STREAMING_H_

/*
** This file contains the code for reading TPM packets and copying them to the chyron. This code
** has been largely copied from aurora/StreamingMode.h
**
*/

class StreamingMode {
    void drawFrameTPM() {
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
        backgroundLayer.swapBuffers();
        SERIAL.write(tpm2Acknowledge);
    } 
}

#endif // __STREAMING_H_
