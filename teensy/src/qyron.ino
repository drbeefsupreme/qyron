/*
Firmware for the Teensy 3.6 for the Quantum Metachyron project, a part of the
Too Late Show conspiracy board.
*/

#include <MatrixHardware_Teensy3_ShieldV4.h> //shield firmware
#include <SmartMatrix.h> //HUB75 library
#include <simpleRPC.h> //RPC library

//local source
#include "colorwheel.c" //for feature demo
#include "gimpbitmap.h"

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


//Feature demo layers
#ifdef USE_ADAFRUIT_GFX_LAYERS
  // there's not enough allocated memory to hold the long strings used by this sketch by default, this increases the memory, but it may not be large enough
  SMARTMATRIX_ALLOCATE_GFX_MONO_LAYER(scrollingLayerF, kMatrixWidth, kMatrixHeight, 6*1024, 1, COLOR_DEPTH, kScrollingLayerOptions);
#else
  SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayerF, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
#endif


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
const int defaultScrollOffset = 6; //not sure if i use this
const rgb24 blackColor = {0, 0, 0};
const rgb24 redColor = {0x40, 0, 0};
const rgb24 defaultBackgroundColor = redColor;

bool blinking = false;
bool currentBG = false;

void setup() {
  delay(1000);
  Serial.begin(9600);

  delay(250);
  debug("Setup() starting");
  matrixSetup();  //initializes the matrix and demo layers
  io.begin(Serial); //passes the serial connection to the RPC lib



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
    pack(&scrollingLayerF, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::start), "scrollingLayerF_start: display text on layer F. @a: char* @return: none",

    //speed
    pack(&scrollingLayer1, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer1_speed: change speed on layer 1. @a: unsigned char @return: none",
    pack(&scrollingLayer2, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer2_speed: change speed on layer 2. @a: unsigned char @return: none",
    pack(&scrollingLayer3, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer3_speed: change speed on layer 3. @a: unsigned char @return: none",
    pack(&scrollingLayer4, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer4_speed: change speed on layer 4. @a: unsigned char @return: none",
    pack(&scrollingLayer5, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayer5_speed: change speed on layer 5. @a: unsigned char @return: none",
    pack(&scrollingLayerF, &SMLayerScrolling<RGB_TYPE(COLOR_DEPTH), kScrollingLayerOptions>::setSpeed), "scrollingLayerF_speed: change speed on layer F. @a: unsigned char @return: none",

    //routines
    runFeatureDemo, "runFeatureDemo: runs the feature demo. @a: none @return: none.",

    //background
    setBlackBackground, "setBlackBackground: sets bg to black. @a: none @return: none.",
    setRedBackground, "setRedBackground: sets bg to red. @a: none @return: none.",
    toggleBlinking, "toggleBlinking: sets the background to flash black and red. @a: none @return: none."
  );

  if(blinking == true) {
    if(currentBG == true) {
      setBlackBackground();
      currentBG = false;
    } else {
      setRedBackground();
      currentBG = true;
    }
    delay(200);
  }
}


void matrixSetup() {
  matrix.addLayer(&backgroundLayer);
  matrix.addLayer(&scrollingLayer1);
  matrix.addLayer(&scrollingLayer2);
  matrix.addLayer(&scrollingLayer3);
  matrix.addLayer(&scrollingLayer4);
  matrix.addLayer(&scrollingLayer5);

  //features demo layers
  matrix.addLayer(&scrollingLayerF);
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


void setBlackBackground() {
  backgroundLayer.fillScreen(blackColor);
  backgroundLayer.swapBuffers();
}

void setRedBackground() {
  backgroundLayer.fillScreen(redColor);
  backgroundLayer.swapBuffers();
}

void toggleBlinking() {
  if(blinking == true) {
    blinking = false;
  } else {
    blinking = true;
  }
}




//from FeatureDemo.ino
void drawBitmap(int16_t x, int16_t y, const gimp32x32bitmap* bitmap) {
  for(unsigned int i=0; i < bitmap->height; i++) {
    for(unsigned int j=0; j < bitmap->width; j++) {
      rgb24 pixel = { bitmap->pixel_data[(i*bitmap->width + j)*3 + 0],
                      bitmap->pixel_data[(i*bitmap->width + j)*3 + 1],
                      bitmap->pixel_data[(i*bitmap->width + j)*3 + 2] };

      backgroundLayer.drawPixel(x + j, y + i, pixel);
    }
  }
}

#define DEMO_INTRO              1
#define DEMO_DRAWING_INTRO      1
#define DEMO_DRAWING_PIXELS     1
#define DEMO_DRAWING_LINES      1
#define DEMO_DRAWING_TRIANGLES  1
#define DEMO_DRAWING_CIRCLES    1
#define DEMO_DRAWING_RECTANGLES 1
#define DEMO_DRAWING_ROUNDRECT  1
#define DEMO_DRAWING_FILLED     1
#define DEMO_FILL_SCREEN        1
#define DEMO_DRAW_CHARACTERS    1
#define DEMO_FONT_OPTIONS       1
#define DEMO_MONO_BITMAP        1
#define DEMO_SCROLL_COLOR       1
#define DEMO_SCROLL_MODES       1
#define DEMO_SCROLL_SPEED       1
#define DEMO_SCROLL_FONTS       1
#define DEMO_SCROLL_POSITION    1
#define DEMO_SCROLL_ROTATION    1
#define DEMO_BRIGHTNESS         1
#define DEMO_RAW_BITMAP         1
#define DEMO_COLOR_CORRECTION   1
#define DEMO_BACKGND_BRIGHTNESS 1
#define DEMO_INDEXED_LAYER      1
#define DEMO_REFRESH_RATE       1
#define DEMO_READ_PIXEL         1

void runFeatureDemo() {
    backgroundLayer.fillScreen(defaultBackgroundColor);
    backgroundLayer.swapBuffers();

    scrollingLayerF.setColor({0xff, 0xff, 0xff});
    scrollingLayerF.setMode(wrapForward);
    scrollingLayerF.setSpeed(40);
    scrollingLayerF.setFont(font6x10);
    scrollingLayerF.start("SmartMatrix Demo", 1);

}
