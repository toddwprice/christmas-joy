#include <FastLED.h>
#include <math.h>

// How many leds in your strip?
#define NUM_LEDS 33
#define NUM_STEPS 37
#define DOTTED_J_PIXEL 23
#define PAUSE_PIXEL -1
#define PAUSE_PIXEL_SHORTER -2
#define J_DOT_BLINKS 3

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 0
// #define CLOCK_PIN 13
#define BRIGHTNESS 20

// struct defining row+column for each pixel
typedef struct Pixel {
  int id;
  int col;  // 28 cols
  int row;  // 13 rows
} Pixel;


// Define the array of leds
CRGB leds[NUM_LEDS];


//Pixel pixels[] = {
//  { 32, 1, 9 },
//  { 31, 2, 10 },
//  { 30, 3, 11 },
//  { 23, 4, 1 },
//  { 24, 4, 5 },
//  { 25, 4, 6 },
//  { 26, 4, 7 },
//  { 27, 4, 8 },
//  { 28, 4, 9 },
//  { 29, 4, 10 },
//  { 16, 7, 6 },
//  { 15, 7, 7 },
//  { 14, 7, 8 },
//  { 22, 8, 5 },
//  { 13, 8, 9 },
//  { 21, 9, 5 },
//  { 12, 9, 9 },
//  { 20, 10, 5 },
//  { 11, 10, 9 },
//  { 19, 11, 6 },
//  { 18, 11, 7 },
//  { 17, 11, 8 },
//  { 8, 14, 5 },
//  { 0, 14, 12 },
//  { 9, 15, 6 },
//  { 2, 15, 10 },
//  { 1, 15, 11 },
//  { 10, 16, 7 },
//  { 4, 16, 8 },
//  { 3, 16, 9 },
//  { 5, 17, 7 },
//  { 6, 18, 6 },
//  { 7, 19, 5 }
//};


// store current hue
static uint8_t hue;

// HANDWRITING
// Order of pixels -- "-1" means "pause between strokes", "-2" means "shorter pause between strokes"
int handwritten[NUM_STEPS] = { 24, 25, 26, 27, 28, 29, 30, 31, 32, PAUSE_PIXEL, 16, 15, 14, 13, 12, 11, 17, 18, 19, 20, 21, 22, PAUSE_PIXEL, 8, 9, 10, PAUSE_PIXEL_SHORTER, 7, 6, 5, 4, 3, 2, 1, 0, PAUSE_PIXEL };

void setup() {
  //Serial.begin(115200);
  //Serial.println("resetting");
  LEDS.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void dimAll() {
  randomSeed(analogRead(4));
  int which = (rand() % 10) + 1;

  if (which <= 5) {
    // fade to black
    int delayAmount = 12;
    for (int b = BRIGHTNESS; b >= 0; b--) {
      LEDS.setBrightness(b);
      FastLED.show();
      delay(delayAmount);
      delayAmount = pow(delayAmount, 0.98);
    }  

    for (int i=0; i < NUM_LEDS; i++) { leds[i] = CRGB::Black; }
  }
  else {
    // randomly set pixels to Black
    int indexes[NUM_LEDS];
  
    // init array
    for (int i=0; i < NUM_LEDS; i++) {
      indexes[i] = i;      
    }
  
    // swap every pixel index with a random other index
    for (int i=0; i < NUM_LEDS; i++) {
      int randIndex = random(0, NUM_LEDS - 1);
      int temp = indexes[i];
      indexes[i] = indexes[randIndex];
      indexes[randIndex] = temp;
    }
  
    // set pixels one by one to Black
    int b = NUM_LEDS;
    for (int i=0; i < NUM_LEDS; i++) {
      leds[indexes[i]] = CRGB::Black;
      LEDS.setBrightness(b);
      FastLED.show();
      delay(15);
      b--;
    }

    LEDS.setBrightness(0);  
  }

  delay(2000);
}

void handwritingStyle() {
  LEDS.setBrightness(BRIGHTNESS);
  hue = random(0, 255);
  float delayAmount = 30;  // amount to delay
  int hueFactor = random(1, 25);


  for (int i = 0; i <= (NUM_STEPS) - 1; i++) {
    if (handwritten[i] == PAUSE_PIXEL) {
      // pause between "strokes"
      delay(500);
      delayAmount = 30;  // reset beginning of next stroke's delay
    } else if (handwritten[i] == PAUSE_PIXEL_SHORTER) {
      // pause between "strokes" -- SHORTER
      delay(300);
      delayAmount = 18;  // reset beginning of next stroke's delay
    } else {
      hueFactor = pow(hueFactor, 1.02);
      leds[handwritten[i]] = CHSV(hue++ + hueFactor, 255, 255);
      FastLED.show();
      delay(delayAmount);
      delayAmount = pow(delayAmount, 0.97);  // decrease delay to show a linear acceleration
    }
  }
}

void dotTheJ() {
  randomSeed(analogRead(5));
  int which = (rand() % 3) + 1;
  int transitionDelay = random(300, 700);

  // do fade style 2/3 of the time
  if (which == 1 || which == 2) {
    // hue change style
    delay(transitionDelay);
    for (int s = 80; s <= 255; s += 2) {
      leds[DOTTED_J_PIXEL] = CHSV(hue * 1.05, 255, s);
      FastLED.show();
      delay(10);
    }
  }
  else {
    // blink style
    
    for (int i = 0; i < J_DOT_BLINKS; i++) {
      delay(transitionDelay);
      for (int s = 80; s <= 255; s += 10) {
        leds[DOTTED_J_PIXEL] = CHSV(hue * 1.05, 255, s);
        FastLED.show();
        delay(1);
      }
      if (i < J_DOT_BLINKS - 1) {
        delay(transitionDelay);
        leds[DOTTED_J_PIXEL] = CRGB::Black;
        FastLED.show();
      }
    }
  }
}

void brightenAll() {
  // brighten all
  int delayAmount = 40;
  for (int b = BRIGHTNESS; b <= BRIGHTNESS * 1.5; b++) {
    LEDS.setBrightness(b);
    FastLED.show();
    delay(delayAmount);
    delayAmount = pow(delayAmount, 1.05);
  }
}

void endTheAnimation() {
  delay(1000);
  brightenAll();
  delay(2000);
  dimAll();
}

void loop() {
  handwritingStyle();
  dotTheJ();
  endTheAnimation();
}
