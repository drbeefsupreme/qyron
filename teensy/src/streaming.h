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
        rgb24 *buffer = backgroundLayer.backBuffer();
    } 
}

#endif // __STREAMING_H_
