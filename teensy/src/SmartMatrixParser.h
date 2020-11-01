#ifndef __SMARTMATRIXPARSER_H_
#define __SMARTMATRIXPARSER_H_

#include <string.h>
#include <stdlib.h>

/*
** SmartMatrixParser translates received commands into
** SmartMatrix API calls
*/

class SmartMatrixParser {
private:

    const short numChars;
    char *commandFromPC;
    char *stringFromPC;

public:

    SmartMatrixParser(short numCharsIn = 1024): numChars(numCharsIn) {
//        this->numChars = numCharsIn;
        char *commandFromPC = (char*)calloc(this->numChars, sizeof(char));
        char *stringFromPC = (char*)calloc(this->numChars, sizeof(char));
    }

    void handleParsedData() {
        debug("handleParsedData()");
        if (strcmp(commandFromPC, "layer1") == 0) {
            scrollingLayer1.start(stringFromPC, -1);
        } else if (strcmp(commandFromPC, "layer2") == 0) {
            scrollingLayer2.start(stringFromPC, -1);
        } else if (strcmp(commandFromPC, "layer3") == 0) {
            scrollingLayer3.start(stringFromPC, -1);
        } else if (strcmp(commandFromPC, "layer4") == 0) {
            scrollingLayer4.start(stringFromPC, -1);
        } else if (strcmp(commandFromPC, "layer5") == 0) {
            scrollingLayer5.start(stringFromPC, -1);
        } else {
            debug("invalid command");
        }

    }

    //takes in an input string and breaks it into a command and arguments and stores it in the object
    void parseData(char* inputString) {

        char * strtokIndx;

        debug("parseData() inputString:");
        debug(inputString);
        strtokIndx = strtok(inputString, "+"); //get the first part - the command
        debug("after strtok");
        strcpy(this->commandFromPC, strtokIndx); // copy the command to the command buffer commandFromPC
        debug("strcpy");
        debug(commandFromPC);

        strtokIndx = strtok(NULL, "+"); //continueparsing from where the previous call left off
        strcpy(this->stringFromPC, strtokIndx);
    }
};




#endif // __SMARTMATRIXPARSER_H_
