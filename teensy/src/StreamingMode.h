#ifndef __STREAMING_H_
#define __STREAMING_H_


#include <string.h>
#include <SmartMatrixParser.h>

/*
** StreamingMode is responsible for reading and parsing commands received over the serial connection.
**
** command format:
** <command+text+>
** e.g.
** <print+bweh skrrter+>
**
*/

class StreamingMode {
private:

public:

    static const byte numChars = 32;
    char receivedChars[numChars];
    char tempChars[numChars];        // temporary array for use when parsing

    boolean newData = false;
    SmartMatrixParser * smParser;

    void setParser(SmartMatrixParser * smPP) {
            smParser = smPP;
    }

    void recvWithStartEndMarkers() {
       static boolean recvInProgress = false;
       static byte ndx = 0;

       char startMarker = '<';
       char endMarker = '>';

       char rc;

       while (SERIAL.available() > 0 && newData == false) {
         rc = SERIAL.read();

         if (recvInProgress == true) {
           if (rc != endMarker) {
             receivedChars[ndx] = rc;
             ndx++;
             if (ndx >= numChars) {
               ndx = numChars - 1;

             }
           } else {
             receivedChars[ndx] = '\0';
             recvInProgress = false;
             ndx = 0;
             newData = true;
           }
         }
         else if (rc == startMarker) {
           recvInProgress = true;
         }
       }
    }

    boolean newHandleStream() {
        recvWithStartEndMarkers();
        if (newData == true) {
            debug("new Handle stream");
            strcpy(tempChars, receivedChars);
            debug("tempChars:");
            debug(tempChars);
            smParser->parseData(tempChars);
            debug("smParser->parseData done");
            smParser->handleParsedData();
            newData = false;
            return true;
        } else {
            return false;
        }
    }


};



#endif // __STREAMING_H_
