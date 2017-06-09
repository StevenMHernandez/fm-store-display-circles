#ifndef BASE_PATTERN_H
#define BASE_PATTERN_H

#include "stdint.h"

struct pattern
{
  uint8_t length;
  uint8_t *frames;
  uint8_t fade_delay;
  uint8_t frame_delay;
};

#endif
