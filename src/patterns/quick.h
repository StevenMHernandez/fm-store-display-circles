#include "base_pattern.h"

uint8_t quick_pattern[] = {
  0b11111111,
  0b00000000,
  0b00000000,

  0b00000000,
  0b11111111,
  0b00000000,

  0b00000000,
  0b00000000,
  0b11111111,

  0b10101010,
  0b10101010,
  0b10101010,

  0b01010101,
  0b01010101,
  0b01010101,
};

struct pattern quick() {
  struct pattern p;

  p.length = 5;
  p.frames = quick_pattern;
  p.fade_delay = 3;
  p.frame_delay = 8;

  return p;
}
