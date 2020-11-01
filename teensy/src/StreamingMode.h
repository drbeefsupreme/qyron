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

// Commands
#define tpm2Layer1 0x01
#define tpm2Layer2 0x02
#define tpm2BeginText 0x03
#define tpm2EndText 0xaa


/*
** This file contains the code for reading TPM packets and copying them to the chyron. This code
** has been largely copied from aurora/StreamingMode.h
**
*/

#include <Arduino.h>

class StreamingMode {
private:
    uint32_t lastData;

    byte numChars;

    char startMarker, endMarker, receivedChars, tempChars, commandFromPC, stringFromPC;


    void drawFrameTPM2();

    void recvWithStartEndMarkers();

    void handleParsedData();

    //split the data into parts
    void parseData();

public:

    StreamingMode();

    bool newData; //tracks whether a command has been put into the buffer ready to be parsed

    void handleStream();

    unsigned int drawFrame();
};


#endif // __STREAMING_H_
