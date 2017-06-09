#include "base_pattern.h"

uint8_t frames_lmsa[] = {
  0b11110000,
  0b00000000,
  0b00000000,

  0b00000000,
  0b11110000,
  0b00000000,

  0b00001111,
  0b00001000,
  0b11110000,

  0b00000000,
  0b00000111,
  0b00001111,
};

struct pattern lets_make_something_awesome_pattern() {
  struct pattern p;

  p.length = 4;
  p.frames = frames_lmsa;
  p.fade_delay = 25;
  p.frame_delay = 25;

  return p;
}
