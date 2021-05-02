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
#define LED_USE_WHITE
#define LED_PIN 25
// #define COLOR_ORDER GRB // Use this with 10 LED RGB //strip
#define COLOR_ORDER RGB // Use this with RGBW strips
#define NUM_LEDS 16

#define BRIGHTNESS 255