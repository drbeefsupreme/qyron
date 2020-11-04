#ifndef __SMARTMATRIXPARSER_H_
#define __SMARTMATRIXPARSER_H_

#include <string.h>
#include <stdlib.h>

/*
** SmartMatrixParser translates received commands into
** SmartMatrix API calls
*/

struct Command {
    char* command;
    void (*callback)(char*);

    //Construct a new command
    Command(char* _command, void (*_callback)(char*)): command(_command), callback(_callback) {}
};

class SmartMatrixParser {
private:

    const short numChars;
    char *commandFromPC;
    char *stringFromPC;

    struct {
        //uint8_t index;
        Command* items[11];
    } commands;

    boolean matchCommand(char* command) {
        return (strcmp(commandFromPC, command) == 0);
    }

public:

    SmartMatrixParser(short numChars = 1024): numChars(numChars) {
        // list initialization necesssary for const
//        this->numChars = numCharsIn;
        //char *commandFromPC = (char*)calloc(this->numChars, sizeof(char));
        //char *stringFromPC = (char*)calloc(this->numChars, sizeof(char));
        //this->commands.items[0]->command = "layer1";
        //this->commands.items[0]->callback = &scrollingLayer1.start;
    }

    void handleParsedData() {
        debug("handleParsedData()");
        if (matchCommand("layer1")) {
            scrollingLayer1.start(stringFromPC, 1);
        } else if (matchCommand("update1")) {
            scrollingLayer1.update(stringFromPC);
        } else if (matchCommand("layer2")) {
            scrollingLayer2.start(stringFromPC, 1);
        } else if (matchCommand("layer3")) {
            scrollingLayer3.start(stringFromPC, 1);
        } else if (matchCommand("layer4")) {
            scrollingLayer4.start(stringFromPC, 1);
        } else if (matchCommand("layer5")) {
            scrollingLayer5.start(stringFromPC, 1);
        } else if (matchCommand("clear")) {
            scrollingLayer1.update("");
            scrollingLayer2.update("");
            scrollingLayer3.update("");
            scrollingLayer4.update("");
            scrollingLayer5.update("");
        } else if (matchCommand("loop1")) {
            scollingLayer1.start(stringFromPC, -1);
        } else if (matchCommand("loop2")) {
            scollingLayer2.start(stringFromPC, -1);
        } else if (matchCommand("loop3")) {
            scollingLayer3.start(stringFromPC, -1);
        } else if (matchCommand("loop4")) {
            scollingLayer4.start(stringFromPC, -1);
        } else if (matchCommand("loop5")) {
            scollingLayer5.start(stringFromPC, -1);
        } else {
            debug("invalid command");
        }

    }

    void handleParsedData2() {
        debug("handleParsedData2");
        //find first matching command and then callback
        for (int i = 0; i < 11; i++) {
            if (strcmp(commandFromPC, this->commands.items[i]->command) == 0) {
                this->commands.items[i]->callback(this->stringFromPC);
                break;
            }
        }
    }

    //takes in an input string and breaks it into a command and arguments and stores it in the object
    void parseData(char* inputString) {

        char * strtokIndx;

        debug("parseData() inputString:");
        debug(inputString);
        strtokIndx = strtok(inputString, "+"); //get the first part - the command
        debug("strtokIndx:");
        debug(strtokIndx);
        debug("after strtok");
        this->commandFromPC = strdup(strtokIndx); // copy the command to the command buffer commandFromPC
        debug("strcpy");
        debug(commandFromPC);

        strtokIndx = strtok(NULL, "+"); //continueparsing from where the previous call left off
        this->stringFromPC = strdup(strtokIndx);
        debug("stringFromPC");
        debug(stringFromPC);
    }
};




#endif // __SMARTMATRIXPARSER_H_
