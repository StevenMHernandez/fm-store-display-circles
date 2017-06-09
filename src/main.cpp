#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include "Arduino.h"
#include "binary_helpers.h"
#include "patterns.h"

#define PIN            6
#define PIR_SENSOR     3

#define NUM_PIXELS     1
#define NUM_ROWS       3
#define NUM_COLUMNS    4

#define LEFT_WINDOW    0
#define RIGHT_WINDOW   1

int window = LEFT_WINDOW;

int frame = -1;
int pattern = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS * NUM_ROWS * NUM_COLUMNS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100;

int color[12];
int value[12];
int direction[12];

void setup() {
  Serial.begin(9600);

  pinMode(PIR_SENSOR, INPUT);

  for (int i=0; i < NUM_ROWS * NUM_COLUMNS; i++) {
    color[i] = random(1,3);
    value[i] = random(0,100);
    direction[i] = random(0,1);
  }

  pixels.begin();
}

bool shouldFade(uint8_t row, uint8_t column) {
  uint8_t offset = window == LEFT_WINDOW ? 4 : 0;
  if ((window == LEFT_WINDOW && row != 1) || (window == RIGHT_WINDOW && row == 1)) {
    // right to left
    return bitAtK(words[frame * NUM_ROWS + (2 - row)], column + offset);
  } else {
    // left to right
    return bitAtK(words[frame * NUM_ROWS+ (2 - row)], (3 - column) + offset);
  }
  return true;
}

void fade(uint8_t frame) {
  int value = 0;

  while (value < 40) {
    value++;

    for(int i = 0; i < NUM_ROWS; i++){
      for(int j = 0; j < NUM_COLUMNS; j++){
        // Should this circle light up?
        if (shouldFade(i, j)) {
          for(int k = 0; k < NUM_PIXELS; k++){
            pixels.setPixelColor(((i * NUM_COLUMNS) + j) * NUM_PIXELS + k, pixels.Color(value, value, value));
          }
        }
      }
    }

    pixels.show();

    delay(10);
  }

  while (value > 0) {
    value--;

    for(int i = 0; i < NUM_ROWS; i++) {
      for(int j = 0; j < NUM_COLUMNS; j++) {
        // Should this circle fade out?
        if (shouldFade(i, j)) {
          for(int k = 0; k < NUM_PIXELS; k++) {
            pixels.setPixelColor(((i * NUM_COLUMNS) + j) * NUM_PIXELS + k, pixels.Color(value, value, value));
          }
        }
      }
    }

    pixels.show();

    delay(10);
  }
}

void loop() {
  // select a random pattern
  pattern = 0;

  // fade in each frame of the pattern
  for (frame = 0; frame < word_frame_count; frame++) {
    Serial.println(frame);
    fade(frame);
  }

  delay(random(1000,5000));
}
