/*
Firmware for the Teensy 3.6 for the Quantum Metachyron project, a part of the
Too Late Show conspiracy board.
*/

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

// Ports
#define SERIAL Serial //Serial port for communication
#define SERIAL_DEBUG Serial //Serial port for debugging


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


//My libraries
#include "StreamingMode.h"
#include "SmartMatrixParser.h"
StreamingMode streamingMode;
SmartMatrixParser smParser;
/*
** Later I should consider abstracting the device as a class, like LedDeviceTpm2 in hyperion.
*/

// constants
const int defaultBrightness = (100*255)/100;
const rgb24 defaultBackgroundColor = {0x40, 0, 0};

void setup() {
  delay(1000);
  SERIAL.begin(115200);

  delay(250);
  debug("Setup() starting");
  demoSetup();  //initializes the matrix and demo layers

  streamingMode.setParser(&smParser);

  delay(3000);

  debug("entering loop...");
}

void loop() {


  //boolean streaming = streamingMode.handleStream();
  boolean streaming = streamingMode.newHandleStream();
}


void demoSetup() {
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer1);
  matrix.addLayer(&scrollingLayer2);
  matrix.addLayer(&indexedLayer);
  matrix.begin();

  matrix.setBrightness(defaultBrightness);

  backgroundLayer.enableColorCorrection(true); //wat this?

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
