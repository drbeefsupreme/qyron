#ifndef __SMARTMATRIXPARSER_H_
#define __SMARTMATRIXPARSER_H_

#include <string.h>

/*
** SmartMatrixParser translates received commands into
** SmartMatrix API calls
*/

class SmartMatrixParser {
private:

public:

    byte numChars;
    char commandFromPC[], stringFromPC[];

    SmartMatrixParser(byte numCharsIn = 64) {
        numChars = numCharsIn;
        commandFromPC[numCharsIn] = {0};
        stringFromPC[numCharsIn] = {0};
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

    //takes in an input string and breaks it into a command and arguments and stores it in the object
    void parseData(char* inputString) {

        char * strtokIndx;

        strtokIndx = strtok(inputString, "+"); //get the first part - the command
        strcpy(commandFromPC, strtokIndx); // copy the command to the command buffer commandFromPC
        debug(commandFromPC);

        strtokIndx = strtok(NULL, "+"); //continueparsing from where the previous call left off
        strcpy(stringFromPC, strtokIndx);
        debug(stringFromPC);
    }
};




#endif // __SMARTMATRIXPARSER_H_
