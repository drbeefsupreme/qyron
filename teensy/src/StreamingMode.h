#ifndef __STREAMING_H_
#define __STREAMING_H_


#include <string.h>

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

          // variables to hold the parsed data
    char commandFromPC[numChars] = {0};
    char stringFromPC[numChars] = {0};

    boolean newData = false;

    void recvWithStartEndMarkers() {
       static boolean recvInProgress = false;
       static byte ndx = 0;

       char startMarker = '<';
       char endMarker = '>';

       char rc;

       while (Serial.available() > 0 && newData == false) {
         rc = Serial.read();

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

    void handleParsedData() {
        debug("handleParsedData()");
        if (strcmp(commandFromPC, "print") == 0) {
            debug("print command received");
            scrollingLayer1.start(stringFromPC, -1);
        } else if (strcmp(commandFromPC, "print2") == 0) {
            debug("layer 2 print command received");
            scrollingLayer2.start(stringFromPC, -1);
        } else {
            debug("invalid command");
        }
    }

     //split the data into parts
    void parseData() {

        char * strtokIndx;

        strtokIndx = strtok(tempChars,"+"); //get the first part - the command
        strcpy(commandFromPC, strtokIndx); // copy the command to the command buffer commandFromPC
        debug(commandFromPC);

        strtokIndx = strtok(NULL, "+"); //continueparsing from where the previous call left off
        strcpy(stringFromPC, strtokIndx);
        debug(stringFromPC);
    }

    boolean handleStream() {
        recvWithStartEndMarkers();
        if (newData == true) {
            debug("newData");
            strcpy(tempChars, receivedChars); //this is used to proect original data since strtok is destructive
            parseData();
            handleParsedData();
            newData = false;
            return true;
        } else {
            return false;
        }
    }


};



#endif // __STREAMING_H_
