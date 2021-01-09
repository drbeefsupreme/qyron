/*
Firmware for the Teensy 3.6 for the Quantum Metachyron project, a part of the
Too Late Show conspiracy board.
*/

#include <MatrixHardware_Teensy3_ShieldV4.h> //shield firmware
#include <SmartMatrix.h> //HUB75 library
#include <simpleRPC.h>

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
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer3, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer4, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer5, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);

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
//#include "StreamingMode.h"
//#include "SmartMatrixParser.h"
//StreamingMode streamingMode;
//SmartMatrixParser smParser(1024);

//simpleRPC io
StreamIO io;

// constants
const int defaultBrightness = (100*255)/100;
const rgb24 defaultBackgroundColor = {0x40, 0, 0};

void setup() {
  delay(1000);
  //SERIAL.begin(115200);
  Serial.begin(9600);

  delay(250);
  debug("Setup() starting");
  demoSetup();  //initializes the matrix and demo layers
  io.begin(Serial); //passes the serial connection to the RPC lib

  //streamingMode.setParser(&smParser);


  delay(3000);
  debug("entering loop...");
}

void loop() {

  //boolean streaming = streamingMode.streamLoop();

  //The following adds SmartMatrix functions to the interface to be passed over the wire to the controller
  interface(
    io,
    //start
    pack(&scrollingLayer1, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::start), "scrollingLayer1_start: display text on layer 1. @a: char* @return: none",
    pack(&scrollingLayer2, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::start), "scrollingLayer2_start: display text on layer 2. @a: char* @return: none",
    pack(&scrollingLayer3, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::start), "scrollingLayer3_start: display text on layer 3. @a: char* @return: none",
    pack(&scrollingLayer4, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::start), "scrollingLayer4_start: display text on layer 4. @a: char* @return: none",
    pack(&scrollingLayer5, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::start), "scrollingLayer5_start: display text on layer 5. @a: char* @return: none",

    //speed
    pack(&scrollingLayer1, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer1_speed: change speed on layer 1. @a: unsigned char @return: none",
    pack(&scrollingLayer2, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer2_speed: change speed on layer 2. @a: unsigned char @return: none",
    pack(&scrollingLayer3, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer3_speed: change speed on layer 3. @a: unsigned char @return: none",
    pack(&scrollingLayer4, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer4_speed: change speed on layer 4. @a: unsigned char @return: none",
    pack(&scrollingLayer5, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer5_speed: change speed on layer 5. @a: unsigned char @return: none"



  );
}


void demoSetup() {
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer1);
  matrix.addLayer(&scrollingLayer2);
  matrix.addLayer(&scrollingLayer3);
  matrix.addLayer(&scrollingLayer4);
  matrix.addLayer(&scrollingLayer5);
  matrix.addLayer(&indexedLayer);


  matrix.begin();

  scrollingLayer1.setMode(wrapForward);
  scrollingLayer2.setMode(bounceForward);
  scrollingLayer3.setMode(bounceReverse);
  scrollingLayer4.setMode(wrapForward);
  scrollingLayer5.setMode(bounceForward);

  scrollingLayer1.setColor({0xff, 0xff, 0xff});
  scrollingLayer2.setColor({0xff, 0x00, 0xff});
  scrollingLayer3.setColor({0xff, 0xff, 0x00});
  scrollingLayer4.setColor({0x00, 0x00, 0xff});
  scrollingLayer5.setColor({0xff, 0x00, 0x00});

  scrollingLayer1.setSpeed(10);
  scrollingLayer2.setSpeed(20);
  scrollingLayer3.setSpeed(40);
  scrollingLayer4.setSpeed(80);
  scrollingLayer5.setSpeed(120);

  scrollingLayer1.setFont(gohufont11b);
  scrollingLayer2.setFont(gohufont11);
  scrollingLayer3.setFont(font8x13);
  scrollingLayer4.setFont(font6x10);
  scrollingLayer5.setFont(font5x7);

  scrollingLayer4.setRotation(rotation270);
  scrollingLayer5.setRotation(rotation90);

  scrollingLayer1.setOffsetFromTop((kMatrixHeight/2) - 5);
  scrollingLayer2.setOffsetFromTop((kMatrixHeight/4) - 5);
  scrollingLayer3.setOffsetFromTop((kMatrixHeight/2 + kMatrixHeight/4) - 5);
  scrollingLayer4.setOffsetFromTop((kMatrixWidth/2 + kMatrixWidth/4) - 5);
  scrollingLayer5.setOffsetFromTop((kMatrixWidth/2 + kMatrixWidth/4) - 5);


  matrix.setBrightness(defaultBrightness);

  backgroundLayer.enableColorCorrection(true); //wat this?

}
