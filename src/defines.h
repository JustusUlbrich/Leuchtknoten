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

#define CONFIG_FILE "/config.json"

 // These need to be known at compile time :(
#define LED_USE_RGBW true
#define LED_GPIO_PIN 25
// #define COLOR_ORDER GRB // Use this with 10 LED RGB //strip
#define LED_COLOR_ORDER RGB // Use this with RGBW strips
