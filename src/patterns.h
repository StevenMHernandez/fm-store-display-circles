#include "Arduino.h"

#include "patterns/lets_make_something_awesome.h"
#include "patterns/type_out_words.h"
#include "patterns/scroll_test.h"
#include "patterns/quick.h"

struct pattern nextPattern(int r) {
  switch(r) {
    case 0:
      return lets_make_something_awesome_pattern();
    case 1:
      return type_out_words();
    case 2:
      return scroll_test();
    case 3:
      return quick();
  }
}

int getRandomPatternIndex() {
    return random(0,4);
}
