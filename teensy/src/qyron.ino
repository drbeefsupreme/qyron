/*
Firmware for the Teensy 3.6 for the Quantum Metachyron project, a part of the
Too Late Show conspiracy board.
*/

#include <TPM2.h> //packet protocol
#include <SmartLEDShieldV4.h> //shield firmware
#include <SmartMatrix3.h> //HUB75 library

/*  SmartMatrix initialization, with settings for the chyron */

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 128;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 16;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_16ROW_MOD8SCAN; // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels, or use SMARTMATRIX_HUB75_64ROW_MOD32SCAN for common 64x64 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

/*
//Initiates THE MATRIX
//Creates a SmartMatrix3 object named matrix with the following attributes via macro
*/
SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);

//Creates an SMLayerBackground
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

//Creates SMLayerScrolling
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer1, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer2, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

//Creates SMLayerIndexed
SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

/*  TPM packet constants */

#define tpm2Header 0xc9
#define tpm2netHeader 0x9c
#define tpm2DataFrame 0xda
#define tpm2Command 0xc0
#define tpm2Answer 0xaa
#define tpm2Footer 0x36
#define tpm2Acknowledge 0xac


// Ports
#define SERIAL Serial //Serial port for communication
#define SERIAL_DEBUG Serial //Serial port for debugging

void setup() {
  delay(1000);
  SERIAL.begin(115200);

  //demoSetup();  //initializes the matrix and demo layers
}

void loop() {
  /*if ( Serial.available() && (Serial.read() == '1')) {
    textLayer1();
  }
  if ( Serial.available() && (Serial.read() == '2')) {
    textLayer2();
  }*/
}


void demoSetup() {
  matrix.addLayer(&scrollingLayer1);
  matrix.addLayer(&scrollingLayer2);
  matrix.begin();

  scrollingLayer1.setMode(wrapForward);
  scrollingLayer2.setMode(bounceForward);

  scrollingLayer1.setColor({0xff, 0xff, 0xff});
  scrollingLayer2.setColor({0xff, 0x00, 0xff});

  scrollingLayer1.setSpeed(10);
  scrollingLayer2.setSpeed(20);

  scrollingLayer1.setFont(gohufont11b);
  scrollingLayer2.setFont(gohufont11);


  scrollingLayer1.setOffsetFromTop((kMatrixHeight/2) - 5);
  scrollingLayer2.setOffsetFromTop((kMatrixHeight/4) - 5);
}

void textLayer1() {
  scrollingLayer1.start("LAYER 1", -1);
}

void textLayer2() {
  scrollingLayer2.start("LAYER 2", -1);
}

//comment this line out to disable debugging

#define DEBUG

#ifdef DEBUG

void debug(const char* str)
{
  SERIAL_DEBUG.println(str);
}

void debug(const char* str, uint16_t val, int fmt = DEC)
{
  SERIAL_DEBUG.print(str);
  SERIAL_DEBUG.println(val, fmt);
}

//finds amount of RAM still free (untested)
int freeRam()
{
   extern int __heap_start, *__brkval;
   int v;
   return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

#else
#  define debug( ... )
#endif
