#include "Arduino.h"

bool bitAtK(uint16_t value, int k) {
    return (value & (1 << k)) != 0;
}
