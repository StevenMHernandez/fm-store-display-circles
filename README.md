# Circle Store Display

Neopixel circles are wired in this fashion. Each circle in the diagram below represents a circle in the display. Lines represent the direction of the wires starting from the arduino and the power source through each circle. Each circle contains 10 leds on an led strip. We are able to change the number of leds from within the code by updating this lines `#define NUM_PIXELS 10`

```
o-o-o-o         o-o-o-o
      |         |
o-o-o-o  +---+  o-o-o-o
|        |   |        |
o-o-o-o  |  -|  o-o-o-o
      |  |   |  |           <- Arduino & Power

  left    door   right
```

Two [Arduinos]() are used for this project. One for the left window, one for the right. Each has the same program onboard other than a single change between each. We must initialize the `window` variable to either be either `LEFT_WINDOW` or `RIGHT_WINDOW`.

```
int window = RIGHT_WINDOW;
```
