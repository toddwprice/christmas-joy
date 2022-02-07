#include <FastLED.h>
#include <math.h>

// How many leds in your strip?
#define NUM_LEDS 61
#define NUM_STEPS 65
#define DOTTED_J_PIXEL 43
#define PAUSE_PIXEL -1
#define PAUSE_PIXEL_SHORTER -2
#define J_DOT_BLINKS 3

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 10
// #define CLOCK_PIN 13
#define BRIGHTNESS 20

// Define the array of leds
CRGB leds[NUM_LEDS];

// struct defining row+column for each pixel
typedef struct Pixel {
  int id;
  int col;  // 28 cols
  int row;  // 13 rows
} Pixel;


// HANDWRITING
// Order of pixels -- "-1" means "pause between strokes", "-2" means "shorter pause between strokes"
int handwritten[NUM_STEPS] = { 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, PAUSE_PIXEL, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,20, 19, 18, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, PAUSE_PIXEL, 12, 13, 14, 15, 16, 17, PAUSE_PIXEL_SHORTER, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, PAUSE_PIXEL};

// store current hue
static uint8_t hue;


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
