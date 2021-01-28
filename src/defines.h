#pragma once

// #define DEBUG_OUT 1
// DIRTY debug
#ifdef DEBUG_OUT
#define debugOutln(a) (Serial.println(a))
#define debugOut(a) (Serial.print(a))
#else
#define debugOutln(a)
#define debugOut(a)
#endif

// LED Setup
#define LED_PIN 25
#define COLOR_ORDER RGB
#define NUM_LEDS 50

#define BRIGHTNESS 16