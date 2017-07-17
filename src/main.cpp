#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#include "binary_helpers.h"
#include "patterns.h"


#define PIN            6
#define IR_INPUT       9

#define NUM_PIXELS     10
#define NUM_ROWS       3
#define NUM_COLUMNS    4

#define MAX_BRIGHT   255

#define LEFT_WINDOW    0
#define RIGHT_WINDOW   1

int window = LEFT_WINDOW;

int frame = -1;
struct pattern pattern;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS * NUM_ROWS * NUM_COLUMNS, PIN, NEO_GRB + NEO_KHZ800);

IRrecv irrecv(IR_INPUT);
IRsend irsend;
decode_results results;

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(0));

  irrecv.enableIRIn();

  pixels.begin();
}

bool shouldFade(uint8_t f, uint8_t row, uint8_t column) {
  uint8_t offset = window == LEFT_WINDOW ? 4 : 0;

  if ((window == LEFT_WINDOW && row != 1) || (window == RIGHT_WINDOW && row == 1)) {
    // right to left
    return bitAtK(pattern.frames[f * NUM_ROWS + (2 - row)], column + offset);
  } else {
    // left to right
    return bitAtK(pattern.frames[f * NUM_ROWS + (2 - row)], (3 - column) + offset);
  }
}

void fade(uint8_t frame) {
  int value = 0;

  while (value < MAX_BRIGHT) {
    value += 2;

    if (value > MAX_BRIGHT) {
      value = MAX_BRIGHT;
    }

    for(int i = 0; i < NUM_ROWS; i++) {
      for(int j = 0; j < NUM_COLUMNS; j++) {
        // Should this circle light up?
        if (shouldFade(frame, i, j)) {
          for(int k = 0; k < NUM_PIXELS; k++) {
            if (frame == 0 || !shouldFade(frame - 1, i, j)) {
              pixels.setPixelColor(((i * NUM_COLUMNS) + j) * NUM_PIXELS + k, pixels.Color(value, value, value));
            } else {
              pixels.setPixelColor(((i * NUM_COLUMNS) + j) * NUM_PIXELS + k, pixels.Color(MAX_BRIGHT, MAX_BRIGHT, MAX_BRIGHT));
            }
          }
        } else {
          for(int k = 0; k < NUM_PIXELS; k++){
            pixels.setPixelColor(((i * NUM_COLUMNS) + j) * NUM_PIXELS + k, pixels.Color(0, 0, 0));
          }
        }
      }
    }

    pixels.show();

    // if (value % 16 == 0) {
    delay(pattern.fade_delay);
    // }
  }

  while (value > 0) {
    value -= 2;

    if (value < 0) {
      value = 0;
    }

    for(int i = 0; i < NUM_ROWS; i++) {
      for(int j = 0; j < NUM_COLUMNS; j++) {
        // Should this circle fade out?
        if (shouldFade(frame, i, j)) {
          for(int k = 0; k < NUM_PIXELS; k++) {
            // fade out if specified,
            // or if it is the last frame in the sequence
            // or if the next frame should be faded
            if (pattern.length - 1 == frame || !shouldFade(frame + 1, i, j)) {
              pixels.setPixelColor(((i * NUM_COLUMNS) + j) * NUM_PIXELS + k, pixels.Color(value, value, value));
            }
          }
        }
      }
    }

    pixels.show();

    // if (value % 16 == 0) {
    delay(pattern.fade_delay);
    // }
  }
}

void loop() {
  if (window == LEFT_WINDOW) {
    // left_windo selects a random pattern
    int r = getRandomPatternIndex();
    pattern = nextPattern(r);

    irsend.sendSony(r, 12);
  } else {
    // wait to receive nextPattern from left_window
    while (!irrecv.decode(&results)) {
      // wait
    }

    Serial.println(results.value);

    pattern = nextPattern(results.value);

    irrecv.resume();
  }

  // fade in each frame of the pattern
  for (frame = 0; frame < pattern.length; frame++) {
    Serial.println(frame);
    fade(frame);
    delay(pattern.frame_delay);
  }

  if (window == LEFT_WINDOW) {
    // master
    delay(1000);
  }

  if (window == LEFT_WINDOW) {
    // left_window waits a random amount of time
    // right_window waits for left_window to say go
    delay(random(100,500));
  }
}
