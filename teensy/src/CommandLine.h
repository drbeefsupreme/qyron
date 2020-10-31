#ifndef __COMMANDLINE_H_
#define __COMMANDLINE_H_

/*****************************************************************************

  How to Use CommandLine:
    Create a sketch.  Look below for a sample setup and main loop code and copy and paste it in into the new sketch.

   Create a new tab.  (Use the drop down menu (little triangle) on the far right of the Arduino Editor.
   Name the tab CommandLine.h
   Paste this file into it.

  Test:
     Download the sketch you just created to your Arduino as usual and open the Serial Window.  Typey these commands followed by return:
      add 5, 10
      subtract 10, 5

    Look at the add and subtract commands included and then write your own!


*****************************************************************************
  Here's what's going on under the covers
*****************************************************************************
  Simple and Clear Command Line Interpreter

     This file will allow you to type commands into the Serial Window like,
        add 23,599
        blink 5
        playSong Yesterday

     to your sketch running on the Arduino and execute them.

     Implementation note:  This will use C strings as opposed to String Objects based on the assumption that if you need a commandLine interpreter,
     you are probably short on space too and the String object tends to be space inefficient.

   1)  Simple Protocol
         Commands are words and numbers either space or comma spearated
         The first word is the command, each additional word is an argument
         "\n" terminates each command

   2)  Using the C library routine strtok:
       A command is a word separated by spaces or commas.  A word separated by certain characters (like space or comma) is called a token.
       To get tokens one by one, I use the C lib routing strtok (part of C stdlib.h see below how to include it).
           It's part of C language library <string.h> which you can look up online.  Basically you:
              1) pass it a string (and the delimeters you use, i.e. space and comman) and it will return the first token from the string
              2) on subsequent calls, pass it NULL (instead of the string ptr) and it will continue where it left off with the initial string.
        I've written a couple of basic helper routines:
            readNumber: uses strtok and atoi (atoi: ascii to int, again part of C stdlib.h) to return an integer.
              Note that atoi returns an int and if you are using 1 byte ints like uint8_t you'll have to get the lowByte().
            readWord: returns a ptr to a text word

   4)  DoMyCommand: A list of if-then-elses for each command.  You could make this a case statement if all commands were a single char.
      Using a word is more readable.
          For the purposes of this example we have:
              Add
              Subtract
              nullCommand
*/
/******************sample main loop code ************************************

  #include "CommandLine.h"

  void
  setup() {
  Serial.begin(115200);
  }

  void
  loop() {
  bool received = getCommandLineFromSerialPort(CommandLine);      //global CommandLine is defined in CommandLine.h
  if (received) DoMyCommand(CommandLine);
  }

**********************************************************************************/

//Name this tab: CommandLine.h

#include <string.h>
#include <stdlib.h>
#include <stdio.h> //used for size_t
#include <ctype.h>

//this following macro is good for debugging, e.g.  print2("myVar= ", myVar);
#define print2(x,y) (Serial.print(x), Serial.println(y))


#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '

#define COMMAND_BUFFER_LENGTH        25                        //length of serial buffer for incoming commands
char   CommandLine[COMMAND_BUFFER_LENGTH + 1];                 //Read commands into this buffer from Serial.  +1 in length for a termination char

const char *delimiters            = " ";                    //words in a string separated by space
const char quotation = '"';
const char *commandFinish = "\n";   //only carriage return ends a command

/*************************************************************************************************************
     your Command Names Here
*/
const char *addCommandToken       = "add";                     //Modify here
const char *subtractCommandToken  = "sub";                     //Modify here
const char *returnStringToken = "return";  //just hand back the string over the wire

/*************************************************************************************************************
    getCommandLineFromSerialPort()
      Return the string of the next command. Commands are delimited by return"
      Handle BackSpace character
      Make all chars lowercase
*************************************************************************************************************/

bool getCommandLineFromSerialPort(char* commandLine)
{
  static uint8_t charsRead = 0;                      //note: COMAND_BUFFER_LENGTH must be less than 255 chars long
  //read asynchronously until full command input
  while (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case CR:      //likely have full command in buffer now, commands are terminated by CR and/or LS
      case LF:
        commandLine[charsRead] = NULLCHAR;       //null terminate our command char array
        if (charsRead > 0)  {
          charsRead = 0;                           //charsRead is static, so have to reset
          Serial.println(commandLine);
          return true;
        }
        break;
      case BS:                                    // handle backspace in input: put a space in last char
        if (charsRead > 0) {                        //and adjust commandLine and charsRead
          commandLine[--charsRead] = NULLCHAR;
          Serial << byte(BS) << byte(SPACE) << byte(BS);  //no idea how this works, found it on the Internet
        }
        break;
      default:
        // c = tolower(c);
        if (charsRead < COMMAND_BUFFER_LENGTH) {
          commandLine[charsRead++] = c;
        }
        commandLine[charsRead] = NULLCHAR;     //just in case
        break;
    }
  }
  return false;
}


/* ****************************
   readNumber: return a 16bit (for Arduino Uno) signed integer from the command line
   readWord: get a text word from the command line

*/
int readNumber () {
  char* numTextPtr = strtok(NULL, delimiters);         //K&R string.h  pg. 250
  return atoi(numTextPtr);                              //K&R string.h  pg. 251
}

char* readWord() {
  char* word = strtok(NULL, delimiters);               //K&R string.h  pg. 250
  return word;
}

char* readStringToken() {
  char* displayString = strtok(NULL, delimiters);
  return displayString;
}

void nullCommand(char* ptrToCommandName) {
  print2("Command not found: ", ptrToCommandName);      //see above for macro print2
}



//getting string inside of quote marks
// this takes in pointer to a string and increments s until it finds a quote and records the position,
// then goes until it finds and records the ending point.
const char* split_space_not_quote(const char *s) {
  const char *start;
  int state = ' ';
  while (*s) {
    switch (state) {
      case '\n': // Could add various white-space here like \f \t \r \v
      case ' ': // Consuming spaces
        if (*s == '\"') {
          start = s;
          state = '\"';  // begin quote
        } else if (*s != ' ') {
          start = s;
          state = 'T';
        }
        break;
      case 'T': // non-quoted text
        if (*s == ' ') {
          printstring(start, s);
          state = ' ';
        } else if (*s == '\"') {
          state = '\"'; // begin quote
        }
        break;
      case '\"': // Inside a quote
        if (*s == '\"') {
          state = 'T'; // end quote
        }
        break;
    }
    s++;
  } // end while
  if (state != ' ') {
    printstring(start, s);
  }
}



/****************************************************
   Add your commands here
*/

int addCommand() {                                      //Modify here
  int firstOperand = readNumber();
  int secondOperand = readNumber();
  return firstOperand + secondOperand;
}

int subtractCommand() {                                //Modify here
  int firstOperand = readNumber();
  int secondOperand = readNumber();
  return firstOperand - secondOperand;
}

char* returnStringCommand() {
    char* gotString = readStringToken();
    return gotString;
}


/****************************************************
   DoMyCommand
*/
bool DoMyCommand(char* commandLine) {
  //  print2("\nCommand: ", commandLine);
  int result;
  char* stringResult;

  char* ptrToCommandName = strtok(commandLine, delimiters);
  //  print2("commandName= ", ptrToCommandName);

  if (strcmp(ptrToCommandName, addCommandToken) == 0) {                   //Modify here
    result = addCommand();
    print2(">    The sum is = ", result);

  } else if (strcmp(ptrToCommandName, subtractCommandToken) == 0) {           //Modify here{
      result = subtractCommand();                                       //K&R string.h  pg. 251
      print2(">    The difference is = ", result);
  } else if (strcmp(ptrToCommandName, returnStringToken)) {
      stringResult = returnStringCommand();
      print2(">   The input string is =", stringResult);
  } else {
      nullCommand(ptrToCommandName);
  }
}


#endif // __COMMANDLINE_H_
